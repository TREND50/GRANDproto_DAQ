echo "##### testcalib.sh: checking calibration channels ###########"
sleep 1

REPTEST=$2

i=0
./calib.sh $1 30 30
NRUN=$(<$REPTEST/last_run.txt)
echo "Now analysing R"$NRUN
python ~/GRANDproto/GRANDproto_python/anaTest.py $NRUN $REPTEST $1 1
sleep 1
./calib.sh $1 60 60
sleep 1
NRUN=$(<$DATADIR/last_run.txt)
echo "Now analysing R"$NRUN
python ~/GRANDproto/GRANDproto_python/anaTest.py $NRUN $REPTEST  $1 1
sleep 1
./calib.sh $1 90 90
sleep 1
NRUN=$(<$DATADIR/last_run.txt)
echo "Now analysing R"$NRUN
python ~/GRANDproto/GRANDproto_python/anaTest.py $NRUN $REPTEST $1 1

