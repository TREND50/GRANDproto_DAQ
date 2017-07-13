#!/bin/bash

if  [ $# != 1 ]
then
	echo "Usage: $0 <session name>"
	exit
fi

tmux kill-window -t $1

