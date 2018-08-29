# GRANDproto_DAQ
DAQ software for GRANDProto35

##Required libraries:
boost-asio
tmux

##Build
make clean; make

## To send message to a board
./sendmsg <dest IP addr> <port> <cfg file>

## To run a server to receive message from a board
./trend_server <port> [output file prefix]
TRENDDATA message will be written to prefix.bin (and prefix.txt temporarly)
Other messages will be written to prefix.txt

## Function of the programms
 1. send_msg: send message to an IP address
 2. trend_server: Run a server to listen to message and record it
 3. bcast_msg: legacy, broadcast message over the whole subnetwork
