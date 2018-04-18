#!/bin/bash
### Used for board validations          
#30/06/2017 VELLEYEN Stephane
#22/09/2017 OMH


if [ $# != 1 ]
then
    echo "Usage:" $0 "<board ID>"
    exit
fi

# Used for STAE tests only
#TESTDIR=$HOME/GRANDproto/tests/tests_STAE   #on lpnws4062 
#vardate=$(date "+%y%m%d_%H%M") 
#mkdir -p $TESTDIR/board$1/$vardate
#export REPTEST=$TESTDIR/board$1/$vardate
#echo 'Creating test folder' $REPTEST
#export DATADIR=$REPTEST
#zero=0
#echo $zero >> $DATADIR/last_run.txt
#sleep 1

export REPTEST=$DATADIR
echo 'Data will be written to' $REPTEST

testping=0
testslc=1
testpattern=1
testcalib=0 #2mins
testfreq=0 # 4mins
testtrig=0 #
testth=0  # 7+4 mins

if test $testping -eq 1
  then  ./setAdress.sh $1 $REPTEST 
  if test $? -eq 0  
    then exit
  fi
fi

if test $testslc -eq 1
  then ./testslc.sh $1 $REPTEST
  if test $? -eq 0  
    then exit
  fi
fi

if test $testpattern -eq 1
  then ./testpattern.sh $1 $REPTEST
fi

if test $testcalib -eq 1
  then ./testcalib.sh $1 $REPTEST
fi

if test $testfreq -eq 1
  then ./testfreq.sh $1 $REPTEST
fi

if test $testtrig -eq 1
  then ./testtrig.sh $1 $REPTEST
fi

if test $testth -eq 1
  then 
  ./testthreshn.sh $1 $REPTEST
  ./testthreshs.sh $1 $REPTEST  
fi
