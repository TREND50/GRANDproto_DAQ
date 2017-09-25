#!/bin/bash

if [ $# != 7 ]
then
    echo "Usage:" $0 "<board ID> <Th1-> <Th1+> <Th2-> <Th2+> <Th3-> <Th3+>"
    exit
fi

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
tmux kill-window -t "w"
# Execute run
$DAQDIR/run.sh  1236 192.168.1.1${BOARDID} norm$NRUN.cfg $DATADIR/R$NRUN'_b'$BOARDID.data "w"

# Log run id
mv norm$NRUN.cfg  $DATADIR/R$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
echo "Now killing tmux window w." 
tmux kill-window -t "w"
