#!/bin/bash
set -e


if [ $# -lt 3 ]; then
    echo "Usage: $0 <benchmark_programm_path> <output_path> <number_of_runs> [benchmark_programm_arguments]"
    exit 1
fi

BENCHMARK_PROGRAM=$1
OUTPUT_DIR=$2
NUM_INSTANCES=$3
shift 3

if [ "$BENCHMARK_PROGRAM" == "./bin/complex" ]; then
    NUM_THREADS=$1
    shift 1
fi

# Create the output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Check if benchmark program exists and is executable
if [ ! -f "$BENCHMARK_PROGRAM" ]; then
    echo "Error: Benchmark program $BENCHMARK_PROGRAM not found."
    exit 1
fi

if [ ! -x "$BENCHMARK_PROGRAM" ]; then
    echo "Error: Benchmark program $BENCHMARK_PROGRAM is not executable."
    exit 1
fi

echo "Running $NUM_INSTANCES instances of $BENCHMARK_PROGRAM with arguments '$@'"
echo "Output will be saved in the $OUTPUT_DIR directory"

# Function to run a single benchmark instance
run_single_benchmark() {
    local INSTANCE_ID=$1
    shift
    local INSTANCE_DIR="$OUTPUT_DIR/instance_$INSTANCE_ID"

    # Create instance-specific directory
    mkdir -p "$INSTANCE_DIR"

    # Handle ema-sort-int: generate a unique input file for this instance
    local ADDITIONAL_ARGS=()
    if [ "$BENCHMARK_PROGRAM" == "./bin/ema-sort-int" ]; then
        local INPUT_FILE="$(realpath "$INSTANCE_DIR/file.bin")"
        echo "Instance $INSTANCE_ID: Generating input file $INPUT_FILE..."
        python3 ./utils/generate_numbers.py 1 "$INPUT_FILE"

        # Ensure the file was created successfully
        if [ ! -f "$INPUT_FILE" ]; then
            echo "Error: Input file $INPUT_FILE was not created!"
            exit 1
        fi

        ADDITIONAL_ARGS+=("$@")
        ADDITIONAL_ARGS+=("$INPUT_FILE")
    
    elif [ "$BENCHMARK_PROGRAM" == "./bin/complex" ]; then
        # Generate input files for each thread
        local FILES=()
        for i in $(seq 1 "$NUM_THREADS"); do
            local FILE="$(realpath "$INSTANCE_DIR/file$i.bin")"
            echo "Instance $INSTANCE_ID: Generating input file $FILE..."
            python3 ./utils/generate_numbers.py 1 "$FILE"
            FILES+=("$FILE")
        done

        # Ensure the files were created successfully
        for FILE in "${FILES[@]}"; do
        if [ ! -f "$FILE" ]; then
            echo "Error: Input file $FILE was not created!"
            exit 1
        fi
        done

        ADDITIONAL_ARGS+=("$NUM_THREADS")
        ADDITIONAL_ARGS+=("$@")
        ADDITIONAL_ARGS+=("${FILES[@]}")
    else
        ADDITIONAL_ARGS=("$@")
    fi

    echo "Starting benchmark instance $INSTANCE_ID..."
    
    # perf stat -d -o "$INSTANCE_DIR/perf.log" "$BENCHMARK_PROGRAM" "${ADDITIONAL_ARGS[@]}"
    perf stat -d -e task-clock,context-switches,cache-misses,cache-references,instructions,cycles -o "$INSTANCE_DIR/perf.log" "$BENCHMARK_PROGRAM" "${ADDITIONAL_ARGS[@]}"
    ltrace -c -o "$INSTANCE_DIR/ltrace.log" "$BENCHMARK_PROGRAM" "${ADDITIONAL_ARGS[@]}"
    strace -c -o "$INSTANCE_DIR/strace.log" "$BENCHMARK_PROGRAM" "${ADDITIONAL_ARGS[@]}"

    # Wait for profiling commands to complete
    wait
    echo "Instance $INSTANCE_ID: Benchmark and profiling completed."
}

# Export function and variables for use in subshells
export -f run_single_benchmark
export BENCHMARK_PROGRAM
export OUTPUT_DIR

echo "Starting system-wide top profiling..."
top -b > "$OUTPUT_DIR/top.log" &
TOP_PID=$!

echo "Mpstat started"
mpstat -P ALL 1 > "$OUTPUT_DIR/mpstat.log" &
MPSTAT_PID=$!

echo "Launching $NUM_INSTANCES benchmark instances..."
pids=()
for i in $(seq 1 "$NUM_INSTANCES"); do
    run_single_benchmark "$i" "$@" &
    pids+=($!)
done

# Wait for all instances to complete
wait "${pids[@]}"


echo "Terminating top profiling..."
kill "$TOP_PID"

echo "Terminating mpstat..."
kill "$MPSTAT_PID"

echo "All $NUM_INSTANCES benchmark instances have completed."
