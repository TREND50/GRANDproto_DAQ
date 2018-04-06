#/bin/bash

# Pattern run
# Parameters: pattern mode

if [ $# != 2 ]
then
    echo "Usage:" $0 "<board ID> <pattern type>"
    exit
fi

# Configuration
BOARDID=$1
NRUN=$(<$DATADIR/last_run.txt)
echo 'Present run ID' $NRUN
NRUN=$(($NRUN+1))
echo 'New run ID' $NRUN


if [ $2 = "0"  ]
then
  patterncfg=pattern_zeros.cfg
elif  [ $2 = "1"  ]
then
  patterncfg=pattern_ones.cfg
elif  [ $2 = "2"  ]
then
  patterncfg=pattern_toggle.cfg
elif  [ $2 = "3"  ]
then
  patterncfg=pattern_deskew.cfg
elif  [ $2 = "4"  ]
then
  patterncfg=pattern_sync.cfg    
else  
  echo 'Wrong parameter value. Aborting.'
  exit 0
fi
echo 'Calling ' $patterncfg

# Clean
tmux kill-window -t "w"
# Execute run
$DAQDIR/run.sh  1236 192.168.1.1$BOARDID $patterncfg $DATADIR/P$NRUN'_b'$BOARDID.data "w"


# Log run id
cp $patterncfg  $DATADIR/P$NRUN'_b'1$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
echo "Now killing tmux window w." 
tmux kill-window -t "w"
