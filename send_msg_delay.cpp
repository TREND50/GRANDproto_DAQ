#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <algorithm>
#include <string>
#include <boost/asio.hpp>
#include "trend_message.hpp"
using namespace std;
using boost::asio::ip::udp;

#define VERBOSE

//modify the port here: 4242 is an example
const int port=1234;
void send_msg(const char* IPadr,int port,const std::vector<uint32_t>& data);
void send_msg(const char* IPadr,int port,const base_message& msg)
{
  std::vector<uint32_t> data;
  std::deque<trend_word> dq=msg.to_deque();
  //std::cout<<"====raw content of the message being sent====\n";
  data.push_back(0xaaaaaaaa);
  data.push_back(msg.type_code());
  for(auto& i:dq)
    {
      //std::cout<<std::setw(8)<<std::hex<<i<<std::endl;
      data.push_back(i);
    }
  data.push_back(0xaaaaaaaa);
  //std::cout<<"====the end of the message sent====\n";
  send_msg(IPadr,port,data);
}

void send_msg(const char* IPadr,int port,const std::vector<uint32_t>& data)
{
  boost::system::error_code error;
  boost::asio::io_service io_service;
  std::string str_port=std::to_string(port);
  udp::socket socket(io_service);
  
  // TRENDSlcReq message
     
  socket.open(boost::asio::ip::udp::v4(),error);

  if(!error)
    {
      socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
      socket.set_option(boost::asio::socket_base::broadcast(true));
      boost::asio::io_service io_service;
      udp::resolver resolver(io_service);
      udp::endpoint endpoint=*resolver.resolve({udp::v4(),IPadr,str_port});
      
      
      socket.send_to(boost::asio::buffer(data.data(),data.size()*sizeof(uint32_t)),endpoint);
      socket.close(error);
      //std::cerr<<error<<std::endl;
    }
}


int main(int argc, char* argv[])
{
  if(argc!=4)
    {
      std::cerr<<"Usage:"<<argv[0]<<" <dest addr> <port> <msg.cfg>"<<std::endl;
      return -1;
    }
    
  const char* IPadr = argv[1];
  std::cout << "Board IPadress=" << argv[1] << std::endl;
  std::cout << "Writing to port=" << argv[2] << std::endl;
  /*
  TRENDDAQ msg1(1,1,1,1,12,2,3,5,3,5);
  send_msg(IPadr,std::stoi(argv[2]),msg1);
  TRENDTRIG msg2(01,0,0x00,1,2,3,4,5,6);
  send_msg(IPadr,std::stoi(argv[2]),msg2);
  TRENDSlcReq msg3;
  send_msg(IPadr,std::stoi(argv[2]),msg3);
  */
  
  /*
  TRENDIntReg msg(true,0x00,0x00,0x00,0x00,0x00,0x00,0x16,0x17);
  TRENDDAQ msg1(1,1,1,1,12,2,3,5,3,5);
  std::map<std::string,message_data> msg1_data;
  msg1_data["DAQon"]=1;
  msg1_data["CalOn"]=1;
  msg1_data["RdWrPlus"]=1;
  TRENDDAQ msg1_1(msg1_data);
  
  TRENDTRIG msg2(0,33,0xff,5,5,5,5,5,15);

  TRENDSlcReq msg3;

  std::vector<uint8_t> gpsdata{1,2};
  //cout<<gpsdata.size()<<endl;
  TRENDGPS msg4(0,0x42,0x12,gpsdata);
  //cout<<msg4.to_string()<<std::endl;


  std::vector<uint16_t> data{1,2,3,4,5,6,7,8,9,10,11,12};
  TRENDData msg5(0x01,0x23,0x45,0x67,0x89,0x00,data);


  TRENDSlc msg6(0x12345678,
		0x11,0x22,
		0x33,0x44,
		0x55,0x66,
		0x11,0x22,
		0x33,0x44,
		0x55,0x66,
		0xab);
   */

  //TRENDADC msg0(5400,2000);
  //std::cout << msg0<< std::endl;
  //send_msg(IPadr,std::stoi(argv[2]),msg0);
  /*
  for(int i=0;i<1;++i)
    {
      send_msg(IPadr,std::stoi(argv[2]),msg5);
      usleep(300);
    }
  */

  
  
  std::ifstream ifs(argv[3]);
  std::map<std::string,message_data> msg_map;
  std::string message_type;
  for(;;)
    {
      std::string line;
      std::getline(ifs,line);
      if(!ifs.good())
	{
	  break;
	}
      bool all_blank=true;
      if(std::all_of(line.begin(),line.end(),[](char c){return std::isblank(c);})||line[0]=='#')
	{
	  continue;
	}
      
      if(line=="==BEGIN==")
	{
#ifdef VERBOSE
	  std::cerr<<"New message..."<<std::endl;
#endif 
	  
	  msg_map.clear();
	  message_type="";
	}
      else if(line=="==END==")
	{
#ifdef VERBOSE
	  std::cerr<<"End of message";
#endif
	  auto p=create_message(message_type,msg_map);
	  if(p)
	    {
#ifdef VERBOSE	      
	      std::cerr<<", now sending it."<<std::endl;
#endif
	      send_msg(IPadr,std::stoi(argv[2]),*p);
	      usleep(10000);
	    }
	  else
	    {
#ifdef VERBOSE
	      std::cerr<<"Unknown message type, ignoring it."<<std::endl;
#endif
	    }
	}
      else
	{
	  if(message_type=="")
	    {
	      message_type=line;
	      continue;
	    }
	  istringstream iss(line+" ");
	  std::string key,type;
	  iss>>key>>type;
	  
	  if(type=="SCALAR")
	    {
	      uint64_t value=0;
	      std::string token;
	      iss>>token;
	      if(message_type=="TRENDSlc"&&(key=="VPower2"||key=="VPower3"))
		{
		  constexpr double factor=(+6.9/2.2)*5/(1ul<<12);
		  double x=std::stod(token);
		  value=x/factor;
		}
	      else if(message_type=="TRENDSlc"&&(key=="VPower1"||
						 key=="VPower4"||
						 key=="VPower5"||
						 key=="VPower6"))
		{
		  constexpr double factor=(+24./2.)*5/(1ul<<12);
		  double x=std::stod(token);
		  value=x/factor;

		}
	      else if(message_type=="TRENDSlc"&&(key=="Th1-"||key=="Th1+"||
						 key=="Th2-"||key=="Th2+"||
						 key=="Th3-"||key=="Th3+"))
		{
		  constexpr double factor=.5;
		  double x=std::stod(token);
		  value=x/factor;
		}
	      else if(message_type=="TRENDSlc"&&key=="Temp")
		{
		  double v=std::stod(token)/.0625;
		  uint16_t result=std::abs(v);
		  if(v<0)
		    {
		      result=(~result)&(0b111111111111)+(1<<13);
		    }
		  value=result;
		}
	      else if(token.size()<2)
		{
		  value=std::stoull(token);
		}
	      else
		{
		  if(token[0]=='0'&&(token[1]=='b'||token[1]=='B'))
		    {
		      token[0]='0';
		      token[1]='0';
		      value=std::stoull(token,nullptr,2);
		    }
		  else if(token[0]=='0'&&(token[1]=='x'||token[1]=='X'))
		    {
		      token[0]='0';
		      token[1]='0';
		      value=std::stoull(token,nullptr,16);
		    }
		  else
		    {
		      value=std::stoull(token);
		    }
		}
	      assert(iss.good());
#ifdef VERBOSE
      	      std::cerr<<key<<" "<<type<<" "<<std::hex<<value<<std::endl;
#endif
	      msg_map[key]=message_data(value);
	    }
	  else if(type=="VECTOR")
	    {
	      std::vector<uint64_t> data;
	      for(;;)
		{
		  uint64_t value;
		  iss>>std::hex>>value;
		  if(!iss.good())
		    {
		      break;
		    }
#ifdef VERBOSE
		  std::cerr<<std::hex<<value<<",";
#endif
		  data.push_back(value);
		}
	      msg_map[key]=message_data(data);
	    }
	  else
	    {
	      std::cerr<<"Error, illegal message type:"<<type<<" , neither VECTOR, nor SCALAR"<<std::endl;
	      
	      assert(0);
	    }
	}
    }

  return 0;
}
