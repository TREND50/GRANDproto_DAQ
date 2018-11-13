# Pattern run
# Parameters: pattern mode

if [ $# != 3 ]
then
    echo "Usage:" $0 "<board ID> <Att1> <Att2>"
    exit
fi

# Configuration
BOARDID=$1
NRUN=$(<$DATADIR/last_run.txt)
NRUN=$(($NRUN+1))

if [ "$2" -gt "127" ] || [ "$3" -gt "127" ]
then
  echo "Error: 127 is the max value allowed for attenuation factor. Aborting."
  exit 0
fi
  
awk '/Attr1/{$3='$2'}1;' calibgen.cfg > temp.cfg
awk '/Attr2/{$3='$3'}1;' temp.cfg > calib$NRUN.cfg

# Clean
tmux kill-window -t "w"
# Execute run
$DAQDIR/run.sh  1236 192.168.1.1$BOARDID calib$NRUN.cfg $DATADIR/C$NRUN'_b'$BOARDID.data "w" 500
#for i in {1..500}
#do 
#  echo $NRUN $BOARDID
#  echo 'Event' $i
#  $DAQDIR/run.sh  1236 192.168.1.1$BOARDID calib$NRUN.cfg $DATADIR/C$NRUN'_b'$BOARDID.data "w" 0
#done  

# Log run id
mv calib$NRUN.cfg  $DATADIR/C$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
echo "Now killing tmux window w." 
tmux kill-window -t "w"
