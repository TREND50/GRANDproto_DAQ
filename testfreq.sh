echo "##### testfreq.sh: checking frequency dependency ###########"
sleep 1

REPTEST=$2

i=0
for freq in {20..110..5}
do
  python cmd_tektro.py 2 $freq # Set sine wave with freq
  ./calibext.sh $1   # take data
  NRUN=$(<$REPTEST/last_run.txt)
  echo "Now analysing R"$NRUN
  python ~/GRANDproto/GRANDproto_python/anaTest.py $NRUN $REPTEST $1 4
  sleep 1
done

python ~/GRANDproto/GRANDproto_python/anaFreq.py $1 $REPTEST
