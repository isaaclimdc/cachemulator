#!/bin/bash

# CACHEMULATOR
# Yuyang Guo (yuyangg) and Isaac Lim (idl)

function cleanTmpFiles {
  declare -a UGLYFILES=("*.out" "*.report" "*.tmp")
  for UGLYFILE in "${UGLYFILES[@]}"
  do
    if ls $UGLYFILE &> /dev/null; then
      rm $UGLYFILE
    fi
  done
}

####### Note: Run this from "cachemulator/src" #######

# Make BFS code
cd programs/bfs
make
cd ../..

# Generate Pin trace
cd ../pin
./pin -t pinatrace.so -- ../src/programs/bfs/bfs ../src/programs/bfs/tiny.graph
cp user.trace ../src/traces/bfs.trace
cd ../src

# Build cache
cleanTmpFiles
make clean
make

# Run cache and plot graph for each protocol
declare -a PROTOCOLS=("MSI" "MESI" "MESIF")
for PROTOCOL in "${PROTOCOLS[@]}"
do
  # Run cache
  ./cache -t traces/bfs.trace -p $PROTOCOL

  # Plot graphs
  cd scripts
  ./showBusTraffic.py $PROTOCOL

  cd ..
done

# Clean
cleanTmpFiles
