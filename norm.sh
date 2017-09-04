#!/bin/bash

echo "./norm.sh [BOAR_ID] [Th1+] [Th1-] [Th2+] [Th2-] [Th3+] [Th3-]"

DAQDIR=`dirname $0`
# Pattern run
# Parameters: pattern mode
SESSION_NAME=norm_session

# Configuration
BOARDID=$1
echo $BOARDID
NRUN=$(<$DATADIR/last_run.txt)
NRUN=$(($NRUN+1))
  
sed -e "s/Th1-.*/Th1-\t\tSCALAR\t$2/g" -e "s/Th1+.*/Th1+\t\tSCALAR\t$3/g" \
	-e "s/Th2-.*/Th2-\t\tSCALAR\t$4/g" -e "s/Th2+.*/Th2+\t\tSCALAR\t$5/g" \
	-e "s/Th3-.*/Th3-\t\tSCALAR\t$6/g" -e "s/Th3+.*/Th3+\t\tSCALAR\t$7/g" \
	normgen.cfg > norm${NRUN}.cfg

# Clean
tmux kill-window -t "aaa"
# Execute run
#$DAQDIR/run.sh  1236 192.168.1.$BOARDID norm$NRUN.cfg $DATADIR/R$NRUN'_b'$BOARDID.data
$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} norm$NRUN.cfg $DATADIR/R$NRUN'_b'$BOARDID.data "aaa"
$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} norm$NRUN.cfg $DATADIR/R$NRUN'_b'$BOARDID.data "aaa"

# Log run id
mv norm$NRUN.cfg  $DATADIR/R$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt

