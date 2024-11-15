#!/bin/bash

DATA_DIR="data"

# Define the number of files/processes
n=40

# Number of threads
t=4

mkdir -p $DATA_DIR

for ((i=1; i<=n; i++))
do

    filenames=()

    for ((j=1; j<=t; j++))
    do
        filename="./data/file_${i}_${j}.bin"
        filenames+=("$filename")

        python3 utils/generate_numbers.py 1024 "$filename"&
    done

    ./bin/complex $t 3000 "${filenames[@]}"&

    echo "Started $i process"

done

# Wait for all processes to complete
wait

echo "All processes have completed"