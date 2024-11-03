#!/bin/bash

SRC_DIR="src"
INCLUDE_DIR="include"
BIN_DIR="bin"

mkdir -p $BIN_DIR

gcc -o $BIN_DIR/main $SRC_DIR/main.c -I$INCLUDE_DIR 
gcc -o $BIN_DIR/bench1 $SRC_DIR/bench1.c -I$INCLUDE_DIR
gcc -o $BIN_DIR/bench2 $SRC_DIR/bench2.c -I$INCLUDE_DIR

python3 scripts/generate_numbers.py 1024 data/myfile.bin
python3 scripts/generate_numbers.py 1 data/mydupfile.bin

echo "Build complete"