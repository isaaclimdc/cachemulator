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

####### Note: Run this from "cachemulator/src"
####### Make sure "userfuncs.in" is in this directory.
####### ./cachemulator.sh <Makefile dir> <executable> <args ...>

PROGDIR="$1"
ARGS="${*:2}"
TRACEFILE="user.trace"

# Build user code
OLDDIR=$(pwd)
cd $PROGDIR
make clean; make
cd $OLDDIR

# Generate Pin trace
../pin/pin -t ../pin/pinatrace.so -- $ARGS

# Build cache
cleanTmpFiles
make

# Run cache and plot graph for each protocol
declare -a PROTOCOLS=("MSI" "MESI" "MESIF")
for PROTOCOL in "${PROTOCOLS[@]}"
do
  # Run cache
  ./emulator -t $TRACEFILE -p $PROTOCOL
  rm *.tmp *.out

  # Plot graphs
  cd scripts
  ./showBusTraffic.py $PROTOCOL

  cd ..
done

# Plot stats graphs
cd scripts
./plotStats.py

# Clean
#cleanTmpFiles
make clean
#rm $TRACEFILE
