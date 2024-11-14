#!/bin/bash

DATA_DIR="data"

# Define the number of files/processes
n=2

mkdir -p $DATA_DIR

for ((i=1; i<=n; i++))
do

    filename="./data/file_$i.bin"

    # generate random numbers
    # python3 scripts/generate_numbers.py 1024 "$filename"&

    ./bin/bench2 2 102400000&
    # ./bin/bench1 3 "$filename"&

    echo "Started process for $filename"

done

# Wait for all processes to complete
wait

echo "All processes have completed"