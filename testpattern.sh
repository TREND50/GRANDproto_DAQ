echo "##### testpattern.sh: checking ADC ###########"
sleep 1

REPTEST=$2

i=0
while ((i<5))
do
  echo "*** Now calling ./pattern.sh" $1 $i
  ./pattern.sh $1 $i
  python test_pattern.py $1 $i $REPTEST 	
  sleep 1
 let i++	
done 			
echo "Pattern test completed"

