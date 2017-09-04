#/bin/bash

# Pattern run
# Parameters: pattern mode


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
  echo 'Wrong parameter value'
fi
echo 'Calling ' $patterncfg

tmux kill-window -t "aaa"

# Execute run
$DAQDIR/run.sh  1236 192.168.1.1$BOARDID $patterncfg $DATADIR/P$NRUN'_b'$BOARDID.data "aaa"
$DAQDIR/run.sh  1236 192.168.1.1$BOARDID $patterncfg $DATADIR/P$NRUN'_b'$BOARDID.data "aaa"


# Log run id
cp $patterncfg  $DATADIR/P$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
tmux kill-window -t "aaa"
