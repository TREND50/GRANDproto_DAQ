# Pattern run
# Parameters: pattern mode

if [ $# != 2 ]
then
    echo "Usage:" $0 "<board ID> <loop>"
    exit
fi

# Configuration
BOARDID=$1
NRUN=$(<$DATADIR/last_run.txt)
echo 'Present run ID' $NRUN
#NRUN=$(($NRUN+1))  No run increment for SLC request


# Execute run
$DAQDIR/run.sh  1235 192.168.1.1$BOARDID slcreq.cfg $DATADIR/S$NRUN'_b'$BOARDID.data "wslc" $2  # Ask for a loop
echo "Now killing server wslc."
tmux kill-window -t "wslc"
