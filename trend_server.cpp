#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <utility>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <deque>
#include <memory>
#include <mutex>

#include "trend_message.hpp"
using boost::asio::ip::udp;

//#define ENDIAN_MISMATCH

class trend_server
{
private:
  std::mutex mutex;
  //std::deque<std::shared_ptr<base_message> > msgbox;
  volatile bool bstop{false};
  std::thread thread;
  std::vector<std::function<void(std::shared_ptr<base_message>)> > handler_list;
public:
  void parse_message(std::deque<trend_word> word_queue)
  {
    //determine the message type here
    assert(word_queue.front()==0xaaaaaaaa);
    assert(word_queue.back()==0xaaaaaaaa);
    word_queue.pop_front();//strip 0xaaaaaaaa
    uint32_t mtype=uint32_t(word_queue.front());
    word_queue.pop_front();//strip message type
    std::shared_ptr<base_message> ptr;
    switch(mtype)
      {
      case 0x00005000:
	std::cerr<<"TRENDDAQ"<<std::endl;
	ptr.reset(new TRENDDAQ);
	break;
      case 0x00005100:
	std::cerr<<"TRENDTRIG"<<std::endl;
	ptr.reset(new TRENDTRIG);
	break;
      case 0x00005200:
	std::cerr<<"TRENDSlcReq"<<std::endl;
	ptr.reset(new TRENDSlcReq);
	break;
      case 0x00005300:
	std::cerr<<"TRENDGPS"<<std::endl;
	ptr.reset(new TRENDGPS);
	break;
      case 0x00005E00:
	std::cerr<<"TRENDIntReg"<<std::endl;
	ptr.reset(new TRENDIntReg);
	break;
      case 0x00005A00:
	//std::cerr<<"TrendData"<<std::endl;
	ptr.reset(new TRENDData);
	break;
      case 0x00005B00:
	std::cerr<<"TrendSlc"<<std::endl;
	ptr.reset(new TRENDSlc);
	break;
      case 0x00005C00:
	std::cerr<<"TrendRdIntReg"<<std::endl;
	ptr.reset(new TRENDRdIntReg);
	break;
      case 0x00005400:
	std::cerr<<"TrendADC"<<std::endl;
	ptr.reset(new TRENDADC);
	break;
      case 0x00005D00:
	std::cerr<<"TrendACK"<<std::endl;
	ptr.reset(new TRENDACK);
	break;
      default:
	std::cerr<<"UNKNOWN"<<std::endl;
      }
    if(ptr)
      {
	ptr->from_deque(word_queue);
	//std::cerr<<ptr->to_string(true)<<std::endl;
	//msgbox.push_back(ptr);
	for(auto& h:handler_list)
	  {
	    h(ptr);
	  }
      }
  }


  void wait_message(udp::socket& socket,unsigned char* buffer,size_t buffer_size,std::deque<trend_word>& word_queue)
  {
    std::lock_guard<std::mutex> guard(mutex);
    //std::cerr<<"entered waiting"<<std::endl;
    std::deque<unsigned char> byte_dq;
    udp::endpoint sender_endpoint;
    int n=socket.receive_from(boost::asio::buffer(buffer, buffer_size),sender_endpoint);
    //std::cerr<<"Received message from "<<sender_endpoint.address().to_string()<<std::endl;
    //std::cerr<< "Length=" << n <<std::endl;
    for(int i=0;i<n;++i)
      {
	byte_dq.push_back(buffer[i]);
      }
    
    while(byte_dq.size()>=4)
      {
	unsigned char bf[4];
	for(int i=0;i<4;++i)
	  {
	    bf[i]=byte_dq.front();
	    byte_dq.pop_front();
	  }
	trend_word tw(bf);
	//std::cerr << tw << std::endl;
#ifdef ENDIAN_MISMATCH
	tw=tw.switch_endian();
#endif
	if(word_queue.empty())
	  {
	    if(tw.is_header_or_trailer())
	      {
		word_queue.push_back(tw);
	      }
	    else
	      {
		std::cerr<<"Error, the first word is not 0xaaaaaaaa, skip it"<<std::endl;
	      }
	  }
	else if(word_queue.size()==1)
	  {
	    if(!tw.is_header_or_trailer())
	      {
		word_queue.push_back(tw);
	      }
	    else
	      {
		std::cerr<<"A header/trailer follows another header/trailer, must be something wrong, maybe missed one header/trailer, skip it"<<std::endl;
	      }
	  }
	else
	  {
	    word_queue.push_back(tw);
	  }
	if(tw.is_header_or_trailer()&&word_queue.size()>2)
	  {
	    std::deque<trend_word> temp_dq;
	    
	    for(auto& x:word_queue)
	      {
		//std::cerr<<x<<std::endl;
	      }
	    temp_dq.swap(word_queue);
	    parse_message(temp_dq);
	  }
      }
    //std::cerr<<"exit waiting"<<std::endl;
  }
  
  
  void listen(int port)
  {
    std::cerr << "trend_server::listen_message" << std::endl;
    boost::asio::io_service io_service;
    udp::socket socket(io_service,udp::endpoint(udp::v4(),port));
    const int buffer_size=65536;
    unsigned char buffer[buffer_size];
    std::deque<trend_word> word_queue;
    //boost::asio::socket_base::broadcast option(false);
    while(!bstop)
      {
	wait_message(socket,buffer,buffer_size,word_queue);
      }
  }

  void run(int port)
  {
    bstop=false;
    if(thread.joinable())
      {
	return;
      }
    thread=std::thread([&](){this->listen(port);});
  }

  void stop()
  {
    bstop=true;
  }

  void wait()
  {
    if(thread.joinable())
      {
	thread.join();
      }
  }
  /*
  std::shared_ptr<base_message> read_msg()
  {
    std::shared_ptr<base_message> result;
    if(mutex.try_lock())
      {
	if(!msgbox.empty())
	  {
	    result=msgbox.front();
	    msgbox.pop_front();
	  }
	mutex.unlock();
      }
    return result;
  }
  */
  void register_handler(std::function<void(std::shared_ptr<base_message>)>  h)
  {
    handler_list.push_back(h);
  }
};

//void listen_message(int port,std::deque<std::shared_ptr<base_message> >& queue)

int main(int argc, char* argv[])
{
  if(argc<2)
    {
      std::cerr<<"Usage:"<<argv[0]<<" <port to be listened> [output file prefix]"<<std::endl;
      std::cerr<<"If no prefix is given, then no output will be written"<<std::endl;
      return -1;
    }
  int port=std::stoi(argv[1]);
  bool write_to_disk=false;
  std::string file_prefix;
  if(argc==3)
    {
      write_to_disk=true;
      file_prefix=argv[2];
    }
  trend_server ts;
  if(write_to_disk)
    {
      ts.register_handler([file_prefix](std::shared_ptr<base_message> pmsg){
	  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	  
	  std::ostringstream oss;
	  oss<<file_prefix;
	  //oss<<std::hex<<pmsg->type_code()<<".txt";
	  std::string fname(oss.str());
	  std::ofstream ofs(fname.c_str(),std::fstream::app);
	  ofs<<"-----------------"<<std::endl;
	  ofs<<std::put_time(std::gmtime(&now_c), "%c %Z") <<std::endl;
	  ofs<<pmsg->to_string(false);
	  ofs<<std::endl;
	});
    }
  //ts.listen(port);
  ts.run(port);
  /*
  for(size_t cnt=0;;)
    {
      std::shared_ptr<base_message> p;
      do
	{
	  p=ts.read_msg();
	  if(p)
	    {
	      ++cnt;
	      std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	      std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	      std::cerr<<p->to_string()<<std::endl;
	      std::ostringstream oss;
	      oss<<std::hex<<p->type_code()<<".txt";
	      std::string fname(oss.str());
	      
	      std::ofstream ofs(fname.c_str(),std::fstream::app);
	      ofs<<"-----------------"<<std::endl;
	      ofs<<"message_id "<<cnt<<std::endl;
	      ofs<<std::put_time(std::gmtime(&now_c), "%c %Z") <<std::endl;
	      ofs<<p->to_string();
	      
	    }
	}while(p);
      //usleep(1000);
    };
  */
  ts.wait();
  return 0;
}
