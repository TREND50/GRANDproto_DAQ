#!/bin/bash

DAQDIR=`dirname $0`
# Pattern run
# Parameters: pattern mode
SESSION_NAME=norm_session

# Configuration
BOARDID=$1
NRUN=$(<$DATADIR/last_run.txt)
NRUN=$(($NRUN+1))
  
for i in {1..1001}
do 
  echo 'Event' $i
  # Execute run
  $DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "aaa"
  #sleep 10
done

# Log run id
cp minbias.cfg  $DATADIR/M$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
tmux kill-window -t "aaa"
