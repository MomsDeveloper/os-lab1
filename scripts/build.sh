#!/bin/bash

SRC_DIR="src"
INCLUDE_DIR="include"
BIN_DIR="bin"

mkdir -p $BIN_DIR

gcc -o $BIN_DIR/main $SRC_DIR/main.c -I$INCLUDE_DIR
gcc -DBENCH1_MAIN -o $BIN_DIR/bench1 $SRC_DIR/bench1.c -I$INCLUDE_DIR
gcc -DBENCH2_MAIN -o $BIN_DIR/bench2 $SRC_DIR/bench2.c -I$INCLUDE_DIR

# compile bench3 that uses bench1 and bench2
gcc -c -o $BIN_DIR/bench1.o $SRC_DIR/bench1.c -I$INCLUDE_DIR
gcc -c -o $BIN_DIR/bench2.o $SRC_DIR/bench2.c -I$INCLUDE_DIR
gcc -o $BIN_DIR/bench3 $SRC_DIR/bench3.c $BIN_DIR/bench1.o $BIN_DIR/bench2.o -I$INCLUDE_DIR -pthread

python3 scripts/generate_numbers.py 1024 data/file.bin
python3 scripts/generate_numbers.py 1 data/dupfile.bin

echo "Build complete"