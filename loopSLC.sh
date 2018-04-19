#/bin/bash

for i in {1..1000}
do
  ./slcreq.sh $1
  sleep 10
done
