#!/bin/bash
# Loop on minBias data for one board BoardID in one dedicated tmux window 
# BUT writing all data in common file M$RunID

if [ $# != 2 ]
then
    echo "Usage:" $0 "<BoardID> <RunID>"
    exit
fi

# Configuration
BOARDID=$1
win="wmb$BOARDID"  # window name
NRUN=$2

# Clean
tmux kill-window -t $win  

#Run
$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN.data $win 200

# Log run id
cp minbias.cfg  $DATADIR/M$NRUN'_b'$BOARDID.cfg

#Kill
echo "Now killing tmux window $win." 
tmux kill-window -t $win

