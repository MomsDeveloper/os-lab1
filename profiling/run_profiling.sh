set -e

if [ $# -ne 3 ]; then
    echo "Usage: $0 <benchmark_programm_path> <output_path> [benchmark_programm_arguments]"
    exit 1
fi

BENCHMARK_PROGRAMM_PATH=$1
OUTPUT_PATH=$2
shift 2

# create output directory
mkdir -p $OUTPUT_PATH

# generate_numbers if benchmark programm is ema-sort-int
if [ $BENCHMARK_PROGRAMM_PATH == "./bin/ema-sort-int" ]; then
    python3 ./utils/generate_numbers.py 1 "$OUTPUT_PATH/file.bin"
    # add generated file to [benchmark_programm_arguments]
    set -- "$@" "$OUTPUT_PATH/file.bin" 
fi

# check if benchmark programm exists
if [ ! -f $BENCHMARK_PROGRAMM_PATH ]; then
    echo "Error: benchmark programm not found"
    exit 1
fi

# check if benchmark programm is executable
if [ ! -x $BENCHMARK_PROGRAMM_PATH ]; then
    echo "Error: benchmark programm is not executable"
    exit 1
fi

perf stat -d -d -d -o $OUTPUT_PATH/perf_stat.txt -- $BENCHMARK_PROGRAMM_PATH $@
echo "perf stat done"

ltrace -c -o $OUTPUT_PATH/ltrace.txt $BENCHMARK_PROGRAMM_PATH $@
echo "ltrace done"

strace -c -o $OUTPUT_PATH/strace.txt $BENCHMARK_PROGRAMM_PATH $@
echo "strace done"

"$BENCHMARK_PROGRAMM_PATH" $@ &
BENCHMARK_PROGRAMM_PID=$!
top -b -n3 > $OUTPUT_PATH/top.txt
wait $BENCHMARK_PROGRAMM_PID
echo "top done"

exit 0