# Pattern run
# Parameters: pattern mode


# Configuration
BOARDID=18
NRUN=$(<$DATADIR/last_run.txt)
NRUN=$(($NRUN+1))

if [ $1 = "0"  ]
then
  patterncfg=pattern_zeros.cfg
elif  [ $1 = "1"  ]
then
  patterncfg=pattern_ones.cfg
elif  [ $1 = "2"  ]
then
  patterncfg=pattern_toggle.cfg
elif  [ $1 = "3"  ]
then
  patterncfg=pattern_deskew.cfg
elif  [ $1 = "4"  ]
then
  patterncfg=pattern_sync.cfg    
else  
  echo 'Wrong parameter value'
fi
echo 'Calling ' $patternfile


# Execute run
$DAQDIR/run.sh  1236 192.168.1.$BOARDID $patterncfg $DATADIR/P$NRUN'_b'$BOARDID.data "aaa"

# Log run id
cp $patterncfg  $DATADIR/P$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
