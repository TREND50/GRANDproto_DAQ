#!/bin/bash

if [ $# != 4 ]
then
    echo "Usage:" $0 "  <server port> <board ip> <cfg> <dump file>"
    exit
fi


if tmux ls 2>/dev/null
then
    for i in `tmux ls|awk '{print $1}'`
    do
	tmux kill-window
    done
fi

SPORT=$1
BPORT=1234  # Fixed port number
BIP=$2
CFG=$3
DUMP_FILE=$4

tmux new -d
tmux split-window -d
tmux next-layout
tmux next-layout
tmux select-pane -t 0
tmux send-keys "./trend_server $SPORT $DUMP_FILE" C-m
tmux select-pane -t 1
for i in {1..1000}
do 
  echo $i
  tmux send-keys "./send_msg $BIP $BPORT  $CFG; "  C-m
done
tmux send-keys "exit" C-m
tmux attach
tmux select-pane -t 0
tmux send-keys "exit" C-m
tmux ls
tmux detach
