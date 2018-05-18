#! /bin/bash
# Script to compare timing on 2 boards
# Trigger on channel 1

./norm.sh $1 30 30 1000 1000 1000 1000 001001  
# This first run will be only used to set parameters on teh 1st board: 
#after the 2nd run is started data is started, its data is anyway channeled to teh same file.
./norm.sh $2 30 30 1000 1000 1000 1000 001001

for i in {1..1000}
  do
  ./slcreq.sh $1
  ./slcreq.sh $2
  sleep 1
done
