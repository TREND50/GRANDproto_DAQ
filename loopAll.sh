#! /bin/bash


for i in {1..100000}  # Infinite loop
do
  NRUN=$(<$DATADIR/last_run.txt)
  NRUN=$(($NRUN+1))
  # Increment run number
  rm $DATADIR/last_run.txt
  echo $NRUN >> $DATADIR/last_run.txt
  
  # Min Bias Run
  cat boardsIn.txt | while read BOARDID
  do
    ./minBiasAll.sh $BOARDID $NRUN & 
  done  
  sleep 30
  
  # Re-start normal acquistion
  cat boardsIn.txt | while read BOARDID
  do
    ./phys.sh $BOARDID 100 100 100 100 100 100 011000 $NRUN
    sleep 0.3
  done
  
  # Take some SLC data
  for j in {1..10}  # Total loop duration: 10 minutes
  do
    cat boardsIn.txt | while read BOARDID
    do
      ./slcreq.sh $BOARDID 0
      sleep 0.3
    done
    sleep 60
  done  
  echo "Now killing data tmux window w." 
  tmux kill-window -t "w"
  echo "Now killing SLC tmux window wslc." 
  tmux kill-window -t "wslc"
done
