#!/bin/bash

if [ $# != 1 ]
then
    echo "Usage:" $0 "<board ID> "
    exit
fi
BOARDID=$1

# Clean
tmux kill-window -t "w"
# Execute run
$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} stop.cfg $DATADIR/R0'_b'$BOARDID.data "w" 0

