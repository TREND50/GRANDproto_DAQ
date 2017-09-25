#!/bin/bash

if [ $# != 5 ]
then
    echo "Usage:" $0 "  <server port> <board ip> <cfg> <dump file> <session name>"
    exit
fi


SPORT=$1
BPORT=1234  # Fixed port number
BIP=$2
CFG=$3
DUMP_FILE=$4
SESSION_NAME=$5

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
sleep 0.01
echo "Now sending message to board."
./send_msg $BIP $BPORT $CFG

