#! /bin/bash
# Script to compare timing on 2 boards
# Trigger on channel 1

#BOARDID=06
cat boardsIn.txt | while read BOARDID
do
 if [ $BOARDID -eq 25 ]
  then
    ./phys.sh $BOARDID 50 50 50 50 1000 1000 000011
  else
    ./phys.sh $BOARDID 200 200 200 200 1000 1000 000011
  fi
done

#./phys.sh $1 200 200 200 200 1000 1000 000011

for i in {1..1000000}
  do
  cat boardsIn.txt | while read BOARDID
  do
    echo "Calling slcreq.sh for board",$BOARDID
    ./slcreq.sh $BOARDID 0
    sleep 10
  done
done
