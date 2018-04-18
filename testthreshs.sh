echo "##### testthreshs.sh: checking threshold on signal ###########"""
sleep 1

REPTEST=$2

# CHECK TRIGGER CH by CH ##########################################

python cmd_tektro.py 3 200  # Square 400mVpp

for trig in 001000 000001 010000 000010 100000 000100
do
  echo '*** Testing trig pattern '$trig
  for th in 5 10 20 50
  do
    echo 'Testing th='$th
    ./norm.sh $1 $th $th $th $th $th $th $trig  # Start run
    sleep 10
    NRUN=$(<$REPTEST/last_run.txt)
    ./norm.sh $1 10 10 10 10 10 10 000000 # Stop run
    fichier='R'$NRUN'_b'$1'.data.txt'
    trigfile=$REPTEST/$fichier
    varls=$(ls $trigfile)
    varls=$?
    if test $varls -eq 0
    then
      nb=$(grep TrigPattern  $trigfile | wc -l)
    else
      nb=0
    fi
    echo $nb ' trigger(s).'
    echo $trig $th $nb >> $REPTEST/threshs_b$1.txt
  done
done
python cmd_tektro.py 0  # Output OFF

python ~/GRANDproto/GRANDproto_python/anaThresh.py $1 s $REPTEST
