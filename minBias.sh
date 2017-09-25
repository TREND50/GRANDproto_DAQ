#!/bin/bash


if [ $# != 1 ]
then
    echo "Usage:" $0 "<board ID>"
    exit
fi

# Configuration
BOARDID=$1
NRUN=$(<$DATADIR/last_run.txt)
NRUN=$(($NRUN+1))

# Clean
tmux kill-window -t "w"  
for i in {1..1000}
do 
  echo 'Event' $i
  # Execute run
  $DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "w"
done

# Log run id
cp minbias.cfg  $DATADIR/M$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
echo "Now killing tmux window w." 
tmux kill-window -t "w"
