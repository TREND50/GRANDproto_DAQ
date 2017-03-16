# Pattern run
# Parameters: pattern mode


# Configuration
BOARDID=18
NRUN=$(<$DATADIR/last_run.txt)
NRUN=$(($NRUN+1))


# Execute run
$DAQDIR/run.sh  1235 192.168.1.$BOARDID slcreq.cfg $DATADIR/S$NRUN'_b'$BOARDID.data "aaa"

