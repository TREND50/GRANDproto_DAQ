#! /bin/bash
# Script to compare timing on 2 boards
# Trigger on channel 1

./phys.sh $1 30 30 1000 1000 1000 1000 001001  
sleep 1
# This first run will be only used to set parameters on teh 1st board: 
#after the 2nd run is started data is started, its data is anyway channeled to the same file.
./phys.sh $2 30 30 1000 1000 1000 1000 001001

for i in {1..10000}
  do
  ./slcreq.sh $1 0
  sleep 1
  ./slcreq.sh $2 0
  sleep 1
done
