#!/bin/bash
#30/06/2017
#VELLEYEN Stephane
#22/09/2017 OMH

if [ $# != 1 ]
then
    echo "Usage:" $0 "<board ID>"
    exit
fi

MAC_adress=18:66:da:2c:4d:9e #on lpnws4062
#MACAD=$(cat /sys/class/net/eth0/address)
#echo $MACAD
TESTDIR=$HOME/GRANDproto/tests   #on lpnws4062 
vardate=$(date "+%y%m%d_%H%M") 
mkdir -p $TESTDIR/board$1/$vardate
export REPTEST=$TESTDIR/board$1/$vardate
echo 'Creating test folder' $REPTEST
export DATADIR=$REPTEST
echo 'Data will be written to' $REPTEST
zero=0
echo $zero >> $DATADIR/last_run.txt
sleep 1


# SET NET CONFIGURATION######################################################################

#while read line;
#do 
#  carte_adre=$(sed -n $1'p' configGedek.txt) #  
#done < configGedek.txt
#echo 'Board 1'$1' MAC adress is: '$carte_adre

#./configureGedek $carte_adre 192.168.1.1$1 192.168.1.1:1236 $MAC_adress 192.168.1.1:1235 $MAC_adress

# CHECK CONNECTION TO BOARD ###############################################################################

nomfic="ping.txt"  # On cree le fichier ping.txt	
varping=$(ping -c 2 192.168.1.1$1)#
#On declare une variable dans laquel on met l'instruction ping 

echo $varping > ./$nomfic 
# On ecrit le resultat de la variable dans le fichier ping.txt


while read line; #Test de la reponse du ping 
do 
  var=$(grep ttl ping.txt) # On essaye d'attaper le mot Unreachable
  var=$?	
done < ping.txt
 

if test $var -eq 1
then #Si la commande renvoie 0
  p1="########## Cannot connect to board 1$1... Abort ##########" #ping fail
  echo $p1
  exit 0
fi
p2="########## Ping to board: sucess :-)##########" #ping sucess
echo $p2	
mv ping.txt $REPTEST


# CHECK SLOW CONTROL ####################################################################################

./slcreq.sh $1  #Execution du slcreq
NRUN=$(<$DATADIR/last_run.txt)
fichier='S'$NRUN'_b'$1'.data'
slcfile=$REPTEST/$fichier
varls=$(ls $slcfile)
varls=$?
if test $varls -eq 0
then
  c="##########DAQ communication with board: success :-)##########"
  echo $c
else
  c1="##########DAQ communication to board failed   ######" 
  echo $c1
  exit 0
fi

#####################################################################################


# CHECK PATTERN CONTROL ####################################################################################
i=0
while ((i<5))
do
  echo "*** Now calling ./pattern.sh" $1 $i
  ./pattern.sh $1 $i
  python test_pattern.py $1 $i $DATADIR  	
#  echo "Result=" $coco
#  if 1==0
#  #if test $? -eq 255
#  then
#    echo "Error pattern type $i"
#    exit 0	
#  fi
 let i++	
done 
				
echo "Pattern test completed"
