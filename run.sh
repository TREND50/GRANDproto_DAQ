#!/bin/sh

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

if tmux ls 2>/dev/null
then
    for i in `tmux ls|awk '{print $1}'`
    do
	tmux kill-window -t $SESSION_NAME
    done
fi


tmux new -d -s $SESSION_NAME
tmux split-window -d
tmux next-layout
tmux next-layout
tmux select-pane -t 0
tmux send-keys "./trend_server $SPORT $DUMP_FILE" C-m
tmux select-pane -t 1
tmux send-keys "./send_msg $BIP $BPORT $CFG; sleep 10; exit" C-m
echo "./trend_server $SPORT $DUMP_FILE" C-m
#tmux attach
