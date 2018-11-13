#!/bin/bash

#if [ $# != 1 ]
#then
#    echo "Usage:" $0 "<board ID> "
#    exit
#fi
#BOARDID=$1

# Clean
tmux kill-window -t "w"
# Execute run
cat boardsIn.txt | while read BOARDID
do
  $DAQDIR/run.sh  1236 192.168.1.1${BOARDID} stop.cfg $DATADIR/R$NRUN'_b'$BOARDID.data "w" 0
done

tmux kill-window -t "w"

