# Pattern run
# Parameters: pattern mode


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

tmux kill-window -t "aaa"

# Execute run
for i in {1..501}
do 
  echo 'Event' $i
  $DAQDIR/run.sh  1236 192.168.1.1$BOARDID calib$NRUN.cfg $DATADIR/C$NRUN'_b'$BOARDID.data "aaa"
done  

# Log run id
mv calib$NRUN.cfg  $DATADIR/C$NRUN'_b'$BOARDID.cfg
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt

tmux kill-window -t "aaa"
