#!/bin/bash


#if [ $# != 1 ]
#then
#    echo "Usage:" $0 "<board ID>"
#    exit
#fi

# Configuration
#BOARDID=$1
NRUN=$(<$DATADIR/last_run.txt)
NRUN=$(($NRUN+1))

# Clean
tmux kill-window -t "wmb"  

cat boardsIn.txt | while read BOARDID
do
  $DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN.data "wmb" 500
  sleep 1
  $DAQDIR/run.sh  1235 192.168.1.1${BOARDID} slcreq.cfg $DATADIR/S$NRUN'_b'$BOARDID.data "wslc" 0
done

#BOARDID=09
#$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "wmb" 500
#BOARDID=10
#$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "wmb" 500
#BOARDID=11
#$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "wmb" 500
#BOARDID=25
#$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "wmb" 500
#BOARDID=27
#$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "wmb" 500
#BOARDID=31
#$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "wmb" 50


#for i in {1..500}
#do 
#  echo 'Event' $i
#  # Execute run
#  $DAQDIR/run.sh  1236 192.168.1.1${BOARDID} minbias.cfg $DATADIR/M$NRUN'_b'$BOARDID.data "wmb" 0
#done

# Log run id
cp minbias.cfg  $DATADIR/M$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
echo "Now killing tmux window wmb." 
tmux kill-window -t "wmb"
echo "Now killing tmux window wslc." 
tmux kill-window -t "wslc"

