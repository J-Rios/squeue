#!/usr/bin/env bash

# Actual script directory path
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

# Example output binary file name
OUT_BIN="${DIR}/example_exe"

# SQueue library path
SQUEUE="${DIR}/../src/"

# Compile
g++ -I $SQUEUE -o $OUT_BIN "${DIR}/generic/main.cpp"
