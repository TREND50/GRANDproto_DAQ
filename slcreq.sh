# Pattern run
# Parameters: pattern mode


# Configuration
BOARDID=$1
NRUN=$(<$DATADIR/last_run.txt)
echo 'Present run ID' $NRUN
#NRUN=$(($NRUN+1))  No run increment for SLC request


# Execute run
$DAQDIR/run.sh  1235 192.168.1.1$BOARDID slcreq.cfg $DATADIR/S$NRUN'_b'$BOARDID.data "bbb"
$DAQDIR/run.sh  1235 192.168.1.1$BOARDID slcreq.cfg $DATADIR/S$NRUN'_b'$BOARDID.data "bbb"

tmux kill-window -t "bbb"
