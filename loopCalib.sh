#!/bin/bash
for i in {0..130..5} 
#for i in {0..0}; 
  do 
  let att1=$i
  for j in {0..0}; 
  #for j in {0..127..1} 
  do
    let att2=$att1
    #let att2=$j*1
    echo "Attenuation:" $att1 $att2
    ./calib.sh $1 $att1 $att2
  done
done
