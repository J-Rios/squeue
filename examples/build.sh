#!/usr/bin/env bash

# Actual script directory path
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

# Example output binary file name
OUT_BIN="${DIR}/example_exe"

# SQueue library path
SQUEUE="${DIR}/../src/"

# Compile
g++ -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion \
    -Wshadow -Wcast-align -Wcast-qual -Wfloat-equal -Wundef -Wswitch-default \
    -Wswitch-enum -Wredundant-decls -Wunreachable-code -Wwrite-strings \
    -Wformat=2 -Wduplicated-cond -Wduplicated-branches -Wlogical-op \
    -Wnull-dereference -Wdouble-promotion -Wvla \
    -I $SQUEUE -o $OUT_BIN "${DIR}/generic/main.cpp"
