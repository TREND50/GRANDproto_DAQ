#!/bin/bash

if [ $# != 6 ]
then
    echo "Usage:" $0 "  <server port> <board ip> <cfg> <dump file> <session name> <loop>"
    exit
fi


SPORT=$1
BPORT=1234  # Fixed port number
BIP=$2
CFG=$3
DUMP_FILE=$4
SESSION_NAME=$5
loop=$6

session_exists=0

if tmux ls 2>/dev/null
then
    for i in `tmux ls|awk -F ':' '{print $1}'`
    do
		if [ $i == $SESSION_NAME ]
		then
			session_exists=1
			#echo "Session exists."
			break
		fi
    done
fi

if [ $session_exists == 0 ]
then
#        echo "Starting session."
	tmux new -d -s $SESSION_NAME
	sleep 0.1
fi

tmux select-pane -t 0
echo "Now starting server."
tmux send-keys "./trend_server $SPORT $DUMP_FILE" C-m
#sleep .5  # Needed on laptop
echo "Now sending message to board."
sleep 0.1

echo "Nloops="$loop
if [ $loop -gt 1 ]
then
  for i in $(seq 1 $loop); do 
    echo "Now sending request" $i ", then sleep 0.1s."
    ./send_msg $BIP $BPORT $CFG
    sleep 0.1
  done
else
  echo "Now sending message to board."
  ./send_msg $BIP $BPORT $CFG
fi
