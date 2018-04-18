echo "##### testslc.sh: checking DAQ comm & SlowControl ###########"""
sleep 3

REPTEST=$2

# CHECK SLOW CONTROL ##########################################
echo "datadir=" $DATADIR
./slcreq.sh $1  #Execution du slcreq with LNA voltage ON
NRUN=$(<$REPTEST/last_run.txt)
fichier='S'$NRUN'_b'$1'.data.txt'
slcfile=$REPTEST/$fichier
varls=$(ls $slcfile)
varls=$?
if test $varls -eq 0
then
  c="##########DAQ communication with board: success :-)##########"
  nedit $slcfile &
  exit 1
else
  c1="##########DAQ communication to board failed   ######" 
  echo $c1
  exit 0
fi
