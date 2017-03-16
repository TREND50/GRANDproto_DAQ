# Pattern run
# Parameters: pattern mode


# Configuration
BOARDID=18
NRUN=$(<$DATADIR/last_run.txt)
NRUN=$(($NRUN+1))
  
awk '/Th1-/{$3='0x$1'}1;' normgen.cfg > temp1m.cfg
awk '/Th1+/{$3='0x$2'}1;' temp1m.cfg > temp1p.cfg
awk '/Th2-/{$3='0x$3'}1;' temp1p.cfg > temp2m.cfg
awk '/Th2+/{$3='0x$4'}1;' temp2m.cfg > temp2p.cfg
awk '/Th3-/{$3='0x$5'}1;' temp2p.cfg > temp3m.cfg
awk '/Th3+/{$3='0x$6'}1;' temp3m.cfg > norm$NRUN.cfg

# Execute run
#$DAQDIR/run.sh  1236 192.168.1.$BOARDID norm$NRUN.cfg $DATADIR/R$NRUN'_b'$BOARDID.data
$DAQDIR/run.sh  1236 192.168.1.$BOARDID norm.cfg $DATADIR/R$NRUN'_b'$BOARDID.data

# Log run id
mv norm$NRUN.cfg  $DATADIR/R$NRUN'_b'$BOARDID.data
rm $DATADIR/last_run.txt
echo $NRUN >> $DATADIR/last_run.txt
