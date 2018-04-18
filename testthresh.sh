echo "##### testthresh.sh: checking threshold ###########"""
sleep 1

REPTEST=$2

# CHECK TRIGGER CH by CH ##########################################

python cmd_tektro.py 0  # Output OFF

NRUN=$(<$REPTEST/last_run.txt)
fichier='R'$NRUN'_b'$1'.data'
datafile=$REPTEST/$fichier
varls=$(ls $datafile)
varls=$?
while test $varls gt 0
do
  echo 'coco'
done

while trig in 001000 000001 010000 000010 100000 000100
do
  echo 'Testing trig pattern '$trig
  ./norm.sh $1 50 50 50 50 50 50 $trig
  sleep 5
  NRUN=$(<$REPTEST/last_run.txt)
  fichier='R'$NRUN'_b'$1'.data.txt'
  trigfile=$REPTEST/$fichier
  varls=$(ls $trigfile)
  varls=$?
  if test $varls -eq 0
  then
    c="##########Trig pattern $trig: success :-) ######"
  else
    c="##########Trig pattern $trig: failure :-( ######" 
  fi
  echo $c
done
./norm.sh $1 10 10 10 10 10 10 000000 # Stop all runs

echo "Now checking high trig rate"
sleep 1
python cmd_tektro.py 4  # Here set high trig rate
sleep 3
./norm.sh $1 50 50 50 50 50 50 100000
sleep 5
./norm.sh $1 10 10 10 10 10 10 000000
sleep 1
./norm.sh $1 50 50 50 50 50 50 000011
sleep 5
./norm.sh $1 10 10 10 10 10 10 000000
