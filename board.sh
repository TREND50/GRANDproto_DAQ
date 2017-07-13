#!/bin/bash
#30/06/2017
#VELLEYEN Stephane


#TESTDIR=/home/trend/Documents/testbench  # on lpnws4250
TESTDIR=/home-local/GRANDproto/GRANDproto/tests   #on lpnws4062 
#DATADIR=/home-local/GRANDproto/GRANDproto/data/  #on lpnws4062
MAC_adress=18:66:da:2c:4d:9e #on lpnws4062

#MACAD=$(cat /sys/class/net/eth0/address)
#echo $MACAD


vardate=$(date "+%y%m%d_%H%M") 
mkdir -p $TESTDIR/board$1/$vardate
export REPTEST=$TESTDIR/board$1/$vardate
echo 'Creating test folder' $REPTEST
export DATADIR=$REPTEST
echo 'Data will be written to' $REPTEST
zero=0
echo $zero 
#echo 0 >> $DATADIR/last_run.txt
cp /home-local/GRANDproto/GRANDproto/data/last_run.txt $DATADIR/last_run.txt
sleep 1

################################################################################


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
  p1="##########Erreur ping##########" #ping fail
  echo $p1
  exit 0
fi
p2="##########Ping to board: sucess :-)##########" #ping sucess
echo $p2	
mv ping.txt $REPTEST


#####################################################################################


while read line;
do 
  carte_adre=$(sed -n $1'p' configGedek.txt) #  
  	  
done < configGedek.txt
echo 'Board MAC adress is:'$carte_adre

#./configureGedek $carte_adre 192.168.1.1$1 192.168.1.1:1236 $MAC_adress 192.168.1.1:1235 $MAC_adress

./slcreq.sh $1	#Execution du slcreq

fichier='S0_b1'$1'.data'
slcfile=$REPTEST/$fichier
varls=$(ls $slcfile)
varls=$?
if test $varls -eq 0
then
  c="##########DAQ communication with board: success :-)##########"
  echo $c
else
  c1="##########Erreur de communication DAQ##########" #Communication fail 
  echo $c1
fi

#####################################################################################

err="Erreur pattern"
i=0


while ((i<5))

do
  python test_pattern.py $1 $i $DATADIR	#ON lance le programme de test des resultat du pattern

  if test $? -eq 255
  then
    echo $err
    exit 0	
  fi

let i++
	
done 
				
test="sucess pattern"
echo $test
