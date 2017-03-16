#!/bin/bash
for i in {1..12}; 
  do 
  echo $i
  for j in {1..1}; 
    do 
    echo $i $j
    ./calib.sh $i $j
    sleep 5
  done
done
