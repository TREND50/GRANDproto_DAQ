echo "##### ping.sh: checking connection to board and setting new IP. ###########"""
sleep 1

REPTEST=$2
#PCMAC_adress="18:66:da:2c:4d:9e" #on lpnws4062
PCMAC_adress="08:00:27:2e:a4:1a" # on XPS13 latptop 
echo "  **** Warning: assuming PC MAC adress is" $PCMAC_adress 
if test 1 -eq 2
then
  # FIRST CHECK CONNECTION TO BOARD WITH INITIAL ADRESS ####

  varping=$(ping -c 2 192.168.1.18)
  echo $varping > "ping.txt"

  while read line; #
  do
    var=$(grep ttl ping.txt) # Grab word "ttl"
    var=$?
  done < ping.txt

  if test $var -eq 1
  then #
    p1="########## Cannot connect to board 1$1... Abort ##########" #ping fail
    echo $p1
    rm ping.txt
    exit 0
  fi
  p2="########## Ping to board 192.168.1.18: success :-)##########" #ping sucess
  echo $p2
  mv ping.txt $REPTEST/ping0.txt
fi

# SET NET CONFIGURATION########################################
cd setAdress
while read line;
do 
  carte_adre=$(sed -n $1'p' configGedek.txt) #  
done < configGedek.txt
echo 'Board 1'$1' registered in configGedek.txt with MAC adress: '$carte_adre
sudo ./configureGedek $carte_adre 192.168.1.1$1 192.168.1.1:1236 $PCMAC_adress 192.168.1.1:1235 $PCMAC_adress
cd ..

# CHECK NEW IP OK #########################################

varping=$(ping -c 2 192.168.1.1$1)#
echo $varping > "ping.txt"

while read line; 
do 
  var=$(grep ttl ping.txt) # Grab word "ttl"
  var=$?	
done < ping.txt 

if test $var -eq 1
then 
  p1="########## Cannot connect to board 1$1... Abort ##########" #ping fail
  rm ping.txt
  exit 0
fi
p2="########## Board $1 now replying with IP=192.168.1.1$1 :-)##########" #ping sucess
echo $p2	
rm ping.txt
exit 1
