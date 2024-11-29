#!/bin/bash

set -e

SRC_DIR="src"
INCLUDE_DIR="include"
BIN_DIR="bin"
TEST_DIR="test"

mkdir -p $BIN_DIR

# -march=native - использует все инструкции процессора, доступные на вашем текущем оборудовании.
# -flto — включение линковки с оптимизацией (Link Time Optimization).
# -funroll-loops — разворачивает циклы
# -fstrict-aliasing — активирует строгую проверку алиасинга.
# -ffast-math — включает оптимизацию математических операций, которая может снизить точность.

CFLAGS="-O3 -march=native -flto -funroll-loops -fstrict-aliasing -ffast-math"
LDFLAGS="-pthread -flto"

gcc $CFLAGS -o $BIN_DIR/shell $SRC_DIR/shell.c -I$INCLUDE_DIR
gcc $CFLAGS -DBENCH1_MAIN -o $BIN_DIR/ema-sort-int $SRC_DIR/ema-sort-int.c -I$INCLUDE_DIR
gcc $CFLAGS -DBENCH2_MAIN -o $BIN_DIR/dedup $SRC_DIR/dedup.c -I$INCLUDE_DIR

gcc $CFLAGS -c -o $BIN_DIR/ema-sort-int.o $SRC_DIR/ema-sort-int.c -I$INCLUDE_DIR
gcc $CFLAGS -c -o $BIN_DIR/dedup.o $SRC_DIR/dedup.c -I$INCLUDE_DIR
gcc $CFLAGS $LDFLAGS -o $BIN_DIR/complex $SRC_DIR/complex.c $BIN_DIR/ema-sort-int.o $BIN_DIR/dedup.o -I$INCLUDE_DIR

# Компиляция тестов
gcc $CFLAGS -o $BIN_DIR/test $TEST_DIR/test.c $SRC_DIR/ema-sort-int.c $BIN_DIR/dedup.o -I$INCLUDE_DIR

echo "Build complete"
