#!/bin/bash

SRC_DIR="src"
INCLUDE_DIR="include"
BIN_DIR="bin"

mkdir -p $BIN_DIR

gcc -o $BIN_DIR/shell $SRC_DIR/shell.c -I$INCLUDE_DIR
gcc -DBENCH1_MAIN -o $BIN_DIR/ema-sort-int $SRC_DIR/ema-sort-int.c -I$INCLUDE_DIR
gcc -DBENCH2_MAIN -o $BIN_DIR/dedup $SRC_DIR/dedup.c -I$INCLUDE_DIR

# compile complex that uses ema-sort-int and dedup
gcc -c -o $BIN_DIR/ema-sort-int.o $SRC_DIR/ema-sort-int.c -I$INCLUDE_DIR
gcc -c -o $BIN_DIR/dedup.o $SRC_DIR/dedup.c -I$INCLUDE_DIR
gcc -o $BIN_DIR/complex $SRC_DIR/complex.c $BIN_DIR/ema-sort-int.o $BIN_DIR/dedup.o -I$INCLUDE_DIR -pthread

python3 utils/generate_numbers.py 1024 data/file.bin
python3 utils/generate_numbers.py 1 data/dupfile.bin

echo "Build complete"