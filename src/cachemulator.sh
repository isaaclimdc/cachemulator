#!/bin/bash

# CACHEMULATOR
# Yuyang Guo (yuyangg) and Isaac Lim (idl)

# Note: Run this from "cachemulator/src"

# Make BFS code
cd programs/bfs
make

# Generate Pin trace
cd ../../../pin
./pin -t pinatrace.so -- ../src/programs/bfs/bfs ../src/programs/bfs/tiny.graph
cp user.trace ../src/traces/bfs.trace

# Run cache
cd ../src
rm *.out *.report *.tmp
make
./cache -t traces/bfs.trace -p MSI

# Plot graphs
cd scripts
./showBusTraffic.py $1