#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

//modify the port here: 4242 is an example

int main(int argc, char* argv[])
{
  if(argc!=2)
    {
      std::cerr<<"Usage:"<<argv[0]<<" <port>"<<std::endl;
      return -1;
    }
  const int port=std::stoi(argv[1]);
  boost::system::error_code error;
  boost::asio::io_service io_service;
  
  udp::socket socket(io_service);
  //Edit the message you want to send in following data[]
  //either you can use unsigned char type or unsigned int
  
  /*
  unsigned char data[]={
    0xaa,0xaa,0xaa,0xaa,
    0x11,0x22,0x33,0x44,
    0x11,0x22,0x33,0x44,
    0xaa,0xaa,0xaa,0xaa
  };
  */

  uint32_t data[]={
    0xaaaaaaaa,
    0x00005000,
    0x11223344,
    0xaaaaaaaa
  };
  socket.open(boost::asio::ip::udp::v4(),error);

  if(!error)
    {
      std::cerr<<"OK"<<std::endl;
      socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
      socket.set_option(boost::asio::socket_base::broadcast(true));

      
      
      boost::asio::ip::udp::endpoint senderEndPoint(boost::asio::ip::address_v4::broadcast(),port);
      socket.send_to(boost::asio::buffer(data,sizeof(data)),senderEndPoint);
      socket.close(error);
      std::cerr<<error<<std::endl;
    }
  return 0;
}
