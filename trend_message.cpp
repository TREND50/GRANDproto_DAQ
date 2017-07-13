#include <utility>
#include "trend_message.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>
std::string ip2str(uint32_t ip)
{
  int ip1=(ip&0xff000000)>>24;
  int ip2=(ip&0x00ff0000)>>16;
  int ip3=(ip&0x0000ff00)>>8;
  int ip4=(ip&0x000000ff);
  std::ostringstream oss;
  oss<<ip1<<'.'<<ip2<<'.'<<ip3<<'.'<<ip4;
  return oss.str();
}


trend_word::trend_word()
  :data(0x00)
{}


trend_word::trend_word(uint32_t n)
  :data(n)
{
}

trend_word::trend_word(const unsigned char* p)
{
  data=(((uint32_t)p[0])<<0)+
    (((uint32_t)p[1])<<8)+
    (((uint32_t)p[2])<<16)+
    (((uint32_t)p[3])<<24);
}

bool trend_word::operator==(const trend_word& rhs)const
{
  return data==rhs.data;
}

bool trend_word::operator==(const uint32_t& rhs)const
{
  return data==rhs;
}

trend_word trend_word::switch_endian()const
{
  uint32_t d0=0x000000ff&data;
  uint32_t d1=0x0000ff00&data;
  uint32_t d2=0x00ff0000&data;
  uint32_t d3=0xff000000&data;

  d0<<=24;
  d1<<=16;
  d2>>=16;
  d3>>=24;
  return trend_word(d0+d1+d2+d3);
}

bool trend_word::is_header_or_trailer()const
{
  return data==0xaaaaaaaa;
}

trend_word::operator uint32_t()const
{
  return data;
}

std::ostream& operator<<(std::ostream& os,const trend_word& tw)
{
  os<<std::setw(8)<<std::hex<<tw.data;
  return os;    
}

base_message::~base_message()
{}

message_data::message_data()
  :scalar_value(0),vector_value(0)
{}

message_data fetch_md(const std::string& name,const std::map<std::string,message_data>& md)
{
  auto i=md.find(name);
  if(i==md.end())
    {
      std::cerr<<"WARNING: Parameter "<<name<<" not given by the cfg map, use default value by filling zero"<<std::endl;
      return message_data();
    }
  else
    {
      return i->second;
    }
}

std::shared_ptr<base_message> create_message(std::string message_type,
					     const std::map<std::string,message_data>& data)
{
  std::shared_ptr<base_message> result;
  if(message_type=="TRENDDAQ")
    {
      result.reset(new TRENDDAQ(data));
    }
  else if(message_type=="TRENDTRIG")
    {
      result.reset(new TRENDTRIG(data));
    }
  else if(message_type=="TRENDSlcReq")
    {
      result.reset(new TRENDSlcReq(data));
    }
  else if(message_type=="TRENDGPS")
    {
      result.reset(new TRENDGPS(data));
    }
  else if(message_type=="TRENDADC")
    {
      result.reset(new TRENDADC(data));
    }
  else if(message_type=="TRENDIntReg")
    {
      result.reset(new TRENDIntReg(data));
    }
  else if(message_type=="TRENDData")
    {
      result.reset(new TRENDData(data));
    }
  else if(message_type=="TRENDSlc")
    {
      result.reset(new TRENDSlc(data));
    }
  else if(message_type=="TRENDRdIntReg")
    {
      result.reset(new TRENDRdIntReg(data));
    }
  else if(message_type=="TRENDACK")
    {
      result.reset(new TRENDACK(data));
    }
  return result;
}

///////////////////
TRENDDAQ::TRENDDAQ()
{
  raw_data.zero=0;
  raw_data.daq_on=0;
  raw_data.ant_on=0;
  raw_data.rd_wr_plus=0;
  raw_data.en_osc=false;
  raw_data.CntrlADC=0;
  raw_data.offst=0;
  raw_data.enable_pd=0;
  raw_data.dis_lna=0;
  raw_data.le=0;
  raw_data.att1=0;
  raw_data.att2=0;
}

TRENDDAQ::TRENDDAQ(bool daq_on,
		   uint8_t ant_on,
		   bool rd_wr_plus,
		   bool en_osc,
		   uint8_t CntrlADC,
		   uint16_t offst,
		   uint8_t enable_pd,
		   uint8_t dis_lna,
		   uint8_t le,
		   uint8_t att1,
		   uint8_t att2)
{
  raw_data.zero=0;
  raw_data.daq_on=daq_on;
  raw_data.ant_on=ant_on;
  raw_data.rd_wr_plus=rd_wr_plus;
  raw_data.en_osc=en_osc;
  raw_data.CntrlADC=CntrlADC;
  raw_data.offst=offst;
  raw_data.enable_pd=enable_pd;
  raw_data.dis_lna=dis_lna;
  raw_data.le=le;
  raw_data.att1=att1;
  raw_data.att2=att2;
}

TRENDDAQ::TRENDDAQ(const std::map<std::string,message_data>& md)
  :TRENDDAQ(fetch_md("DAQon",md),
	    fetch_md("AntOn",md),
	    fetch_md("RdWrPlus",md),
	    fetch_md("EnOSC",md),
	    fetch_md("CntrlADC",md),
	    fetch_md("Offst",md),
	    fetch_md("EnablePD",md),
	    fetch_md("DisLNA",md),
	    fetch_md("LE",md),
	    fetch_md("Attr1",md),
	    fetch_md("Attr2",md))
{}

std::deque<trend_word> TRENDDAQ::to_deque()const
{
  return ::to_deque(raw_data);
}

void TRENDDAQ::from_deque(const std::deque<trend_word>& dq)
{
  raw_data=::from_deque<decltype(raw_data)>(dq);
}

uint32_t TRENDDAQ::type_code()const
{
  return 0x5000;
}

std::string TRENDDAQ::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(use_hex)
    {
      oss<<std::hex;
    }
  oss<<"daq on:"<<raw_data.daq_on<<std::endl
     <<"ant on:"<<raw_data.ant_on<<std::endl
     <<"rd wr plus:"<<raw_data.rd_wr_plus<<std::endl
     <<"en osc:"<<raw_data.en_osc<<std::endl
     <<"CntrlADC"<<raw_data.CntrlADC<<std::endl
     <<"offst:"<<raw_data.offst<<std::endl
     <<"enable pd:"<<raw_data.enable_pd<<std::endl
     <<"dis_lna:"<<raw_data.dis_lna<<std::endl
     <<"le:"<<raw_data.le<<std::endl
     <<"att1:"<<raw_data.att1<<std::endl
     <<"att2:"<<raw_data.att2;
  
	     
  return oss.str();
}
/////////////////////////////
TRENDTRIG::TRENDTRIG(uint8_t ST,
		     uint8_t TrgEn,
		     uint16_t CntrlDAC,
		     uint16_t Th1m,uint16_t Th1p,
		     uint16_t Th2m,uint16_t Th2p,
		     uint16_t Th3m,uint16_t Th3p
		     )
{
  raw_data.ST=ST;
  raw_data.TrgEn=TrgEn;
  raw_data.CntrlDAC=CntrlDAC;
  if(!(CntrlDAC&0x8000))
    {
      raw_data.Th1m=Th1m*2;
      raw_data.Th1p=Th1p*2;
      raw_data.Th2m=Th2m*2;
      raw_data.Th2p=Th2p*2;
      raw_data.Th3m=Th3m*2;
      raw_data.Th3p=Th3p*2;
    }
}

TRENDTRIG::TRENDTRIG(const std::map<std::string,message_data>& md)
  :TRENDTRIG(fetch_md("ST",md),
	     fetch_md("TrgEn",md),
	     fetch_md("CntrlDAC",md),
	     fetch_md("Th1-",md),fetch_md("Th1+",md),
	     fetch_md("Th2-",md),fetch_md("Th2+",md),
	     fetch_md("Th3-",md),fetch_md("Th3+",md))
{}

TRENDTRIG::TRENDTRIG(const TRENDTRIG& rhs)
  :raw_data(rhs.raw_data)
{}

TRENDTRIG& TRENDTRIG::operator=(const TRENDTRIG& rhs)
{
  raw_data=rhs.raw_data;
  return *this;
}

std::deque<trend_word> TRENDTRIG::to_deque()const
{
  if(raw_data.CntrlDAC&0x8000)
    {//shortened
      return std::deque<trend_word>{::to_deque(raw_data)[0]};
      
    }
  else
    {
      return ::to_deque(raw_data);
    }
}

void TRENDTRIG::from_deque(const std::deque<trend_word>& dq)
{
  uint32_t d=dq[0];
  if(d&0x800000)
    {
      //shortened
      (*(uint32_t*)&raw_data)=d;
    }
  else
    {
      raw_data=::from_deque<decltype(raw_data)>(dq);
    }
}

uint32_t TRENDTRIG::type_code()const
{
  return 0x5100;
}

std::string TRENDTRIG::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(use_hex)
    {
      oss<<std::hex;
    }
  oss<<"CntrlDAC:"<<raw_data.CntrlDAC<<std::endl
     <<"TrgEn:"<<raw_data.TrgEn<<std::endl
     <<"ST:"<<raw_data.ST;
  if(!(raw_data.CntrlDAC&0x8000))
    {
      oss<<std::endl;
      oss<<std::setbase(10);
      oss<<"[Th1-,Th1+]:"<<raw_data.Th1m<<" "<<raw_data.Th1p<<std::endl
	 <<"[Th2-,Th2+]:"<<raw_data.Th2m<<" "<<raw_data.Th2p<<std::endl
	 <<"[Th3-,Th3+]:"<<raw_data.Th3m<<" "<<raw_data.Th3p;
    }
  return oss.str();
}
/////////////////////////////
TRENDSlcReq::TRENDSlcReq(const std::map<std::string,message_data>& md)
{}

std::deque<trend_word> TRENDSlcReq::to_deque()const
{
  return std::deque<trend_word>();
}

void TRENDSlcReq::from_deque(const std::deque<trend_word>& dq)
{
}

uint32_t TRENDSlcReq::type_code()const
{
  return 0x5200;
}

std::string TRENDSlcReq::to_string(bool use_hex)const
{
  return std::string("trend slc req");
}
//////////////////////////////
TRENDGPS::TRENDGPS()
{
  header.zeros=0;
  header.RWb=0;
  header.Addr=0x42;
  header.wrd=0;
}

TRENDGPS::TRENDGPS(uint8_t RWb,uint8_t Addr,uint8_t wrd,const std::vector<uint8_t> d)
  :data(d)
{
  header.zeros=0;
  header.RWb=RWb;
  header.Addr=Addr;
  header.wrd=wrd;
}

TRENDGPS::TRENDGPS(const std::map<std::string,message_data>& md)
  :TRENDGPS(fetch_md("RWb",md),
	    fetch_md("Addr",md),
	    fetch_md("wrd",md),
	    fetch_md("data",md))
{
}

std::deque<trend_word> TRENDGPS::to_deque()const
{
  auto result=::to_deque(header);
  for(int i=0;i<data.size()/2;++i)
    {
      auto d1=data[i*2];
      auto d2=data[i*2+1];
      trend_word tw(d1+(((uint32_t)d2)<<8));
      result.push_back(tw);
    }
  if(data.size()%2)
    {
      result.push_back((uint32_t)data.back());
    }
  return result;
}

void TRENDGPS::from_deque(const std::deque<trend_word>& dq)
{
  data.clear();
  header=*((const decltype(header)*)(&dq.front()));
  for(int i=1;i<dq.size();++i)
    {
      uint32_t d=dq[i];
      if(d==0xaaaaaaaa)
	{
	  break;
	}
      uint8_t d1=d&0xff;
      uint8_t d2=(d&0xff00)>>8;
      data.push_back(d1);
      data.push_back(d2);
    }
}

uint32_t TRENDGPS::type_code()const
{
  return 0x5300;
}

std::string TRENDGPS::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(use_hex)
    {
      oss<<std::hex;
    }
  oss<<"RWb:"<<header.RWb<<std::endl
     <<"Addr:"<<header.Addr<<std::endl
     <<"wrd:"<<header.wrd<<std::endl;
  oss<<"data section:"<<"["<<data.size()<<"]"<<std::endl;
  for(int i=0;i<data.size();++i)
    {
      oss<<(int)data[i]<<" ";
    }
  oss<<std::endl;
  return oss.str();
}
///////////////////
TRENDIntReg::TRENDIntReg(bool write,
			 uint32_t bmac,
			 uint32_t bip,
			 uint64_t dmac1,
			 uint32_t dip1,
			 uint64_t dmac2,
			 uint32_t dip2,
			 uint16_t port1,
			 uint16_t port2)
{
  raw_data.rw=write;
  raw_data.AD0=bmac;
  raw_data.AD1=bip;
  raw_data.AD2=dmac1&(0x00000000ffffffff);
  raw_data.AD3=dmac1>>32;
  raw_data.AD4=dip1;
  raw_data.AD5=dmac2&(0x00000000ffffffff);
  raw_data.AD6=dmac2>>32;
  raw_data.AD7=dip2;
  raw_data.AD8=port1;
  raw_data.AD9=port2;
}

TRENDIntReg::TRENDIntReg(const std::map<std::string,message_data>& md)
  :TRENDIntReg(fetch_md("write",md),
	       fetch_md("BoardMAC",md),
	       fetch_md("BoardIP",md),
	       fetch_md("MAC1",md),
	       fetch_md("IP1",md),
	       fetch_md("MAC2",md),
	       fetch_md("IP2",md),
	       fetch_md("PORT1",md),
	       fetch_md("PORT2",md))
{}

TRENDIntReg::TRENDIntReg(const TRENDIntReg& rhs)
  :raw_data(rhs.raw_data)
{}

TRENDIntReg& TRENDIntReg::operator=(const TRENDIntReg& rhs)
{
  raw_data=rhs.raw_data;
  return *this;
}

std::deque<trend_word> TRENDIntReg::to_deque()const
{
  if(raw_data.rw)
    {
      return ::to_deque(raw_data);
    }
  else
    {
      return ::to_deque<uint32_t>(0x00000000);
    }
}

void TRENDIntReg::from_deque(const std::deque<trend_word>& dq)
{
  if(dq[0]==(uint32_t)1)
    {
      raw_data=::from_deque<decltype(raw_data)>(dq);
    }
  else if(dq[0]==(uint32_t)0x00)
    {
      raw_data.rw=::from_deque<uint32_t>(dq);
    }
  else
    {
      std::cerr<<"Unknown message format"<<std::endl;
      std::cerr<<"Y can only =0 or =1"<<std::endl;
      assert(0);
    }
}

uint32_t TRENDIntReg::type_code()const
{
  return 0x5e00;
}

std::string TRENDIntReg::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(raw_data.rw)
    {
      if(use_hex)
	{
	  oss<<std::hex;
	}
      oss<<"write"<<std::endl
	 <<"board mac:"<<std::setbase(16)<<raw_data.AD0<<std::endl
	 <<"board ip:"<<ip2str(raw_data.AD1)<<std::endl
	 <<"dest mac1:"<<std::setbase(16)<<(raw_data.AD2+((uint64_t)raw_data.AD3<<32))<<std::endl
	 <<"dest ip1:"<<ip2str(raw_data.AD4)<<std::endl
	 <<"dest mac2:"<<std::setbase(16)<<(raw_data.AD5+((uint64_t)raw_data.AD6<<32))<<std::endl
	 <<"dest ip2:"<<ip2str(raw_data.AD7)<<std::endl
	 <<"port1:"<<raw_data.AD8<<std::endl
	 <<"port2:"<<raw_data.AD9;
    }
  else
    {
      oss<<"read";
    }
  return oss.str();
}
/////////////////////////////////
TRENDData::TRENDData()
{
  header.IP=0x00;
  header.TS2=0x00;
  header.TS1Trigger=0x00;
  header.TS1PPS=0x00;
  header.SSS=0x00;
  header.EC=0x00;
  header.TrigPattern=0x00;
  header.zero=0x00;
}

TRENDData::TRENDData(uint32_t IP,
		     uint32_t TS2,
		     uint8_t TS1Trigger,
		     uint8_t TS1PPS,
		     uint16_t SSS,
		     uint32_t EC,
		     uint8_t TrigPattern,
		     const std::vector<uint16_t>& d)
  :data(d)
{
  header.IP=IP;
  header.TS2=TS2;
  header.TS1Trigger=TS1Trigger;
  header.TS1PPS=TS1PPS;
  header.SSS=SSS;
  header.EC=EC;
  header.TrigPattern=TrigPattern;
  header.zero=0x00;
}

TRENDData::TRENDData(const std::map<std::string,message_data>& md)
  :TRENDData(fetch_md("IP",md),
	     fetch_md("TS2",md),
	     fetch_md("TS1Trigger",md),
	     fetch_md("TS1PPS",md),
	     fetch_md("SSS",md),
	     fetch_md("EventCounter",md),
	     fetch_md("TrigPattern",md),
	     fetch_md("data",md))
{}

std::deque<trend_word> TRENDData::to_deque()const
{
  std::deque<trend_word> result=::to_deque(header);
  for(int i=0;i<data.size()/2;++i)
    {
      uint16_t d1=data[i*2];
      uint16_t d2=data[i*2+1];
      uint32_t d=(((uint32_t)d2)<<12)+d1;
      result.push_back(d);
    }
  if(data.size()%2)
    {
      result.push_back((uint32_t)data.back());
    }
  return result;
}


void TRENDData::from_deque(const std::deque<trend_word>& dq)
{
  header=::from_deque<decltype(header)>(dq);
  size_t shift=sizeof(header)/4;
  data.clear();
  for(size_t i=shift;i<dq.size();++i)
    {
      uint32_t d=dq[i];
      if(d==0xaaaaaaaa)
	{
	  break;
	}
      uint16_t d1=d&0xfff;
      uint16_t d2=(d&0xfff000)>>12;
      data.push_back(d1);
      data.push_back(d2);
    }
}

uint32_t TRENDData::type_code()const
{
  return 0x5A00;
}

std::string TRENDData::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(use_hex)
    {
      oss<<std::hex;
    }
  oss<<"IP:"<<ip2str(header.IP)<<std::endl
     <<"TS2:"<<header.TS2<<std::endl
     <<"TS1Trigger:"<<header.TS1Trigger<<std::endl
     <<"TS1PPS:"<<header.TS1PPS<<std::endl
     <<"SSS:"<<header.SSS<<std::endl
     <<"EC:"<<header.EC<<std::endl
     <<"TrigPattern:"<<header.TrigPattern<<std::endl;

  for(const auto& i:data)
    {
      oss<<i<<" ";
    }
  oss<<std::endl;
  return oss.str();
}

/////////////////////////////////
TRENDSlc::TRENDSlc()
{
  raw_data.zero1=
    raw_data.zero2=
    raw_data.zero3=
    raw_data.zero4=
    raw_data.zero5=
    raw_data.zero6=
    raw_data.zero7=0;
  raw_data.IP=0;
  raw_data.VPower1=0;
  raw_data.VPower2=0;
  raw_data.VPower3=0;
  raw_data.VPower4=0;
  raw_data.VPower5=0;
  raw_data.VPower6=0;

  raw_data.Th1m=0;
  raw_data.Th1p=0;
  raw_data.Th2m=0;
  raw_data.Th2p=0;
  raw_data.Th3m=0;
  raw_data.Th3p=0;

  raw_data.Temp=0;

  raw_data.TotalTrigRate=0;
  raw_data.Ch1pTrigRate=0;
  raw_data.Ch2pTrigRate=0;
  raw_data.Ch3pTrigRate=0;
  raw_data.Ch1mTrigRate=0;
  raw_data.Ch2mTrigRate=0;
  raw_data.Ch3mTrigRate=0;

  raw_data.MaxCoarse=0;
}


TRENDSlc::TRENDSlc(uint32_t IP,
		   uint16_t VPower1,uint16_t VPower2,
		   uint16_t VPower3,uint16_t VPower4,
		   uint16_t VPower5,uint16_t VPower6,
		   uint16_t Th1m,uint16_t Th1p,
		   uint16_t Th2m,uint16_t Th2p,
		   uint16_t Th3m,uint16_t Th3p,
		   uint16_t Temp,
		   uint32_t TotalTrigRate,
		   uint32_t Ch1pTrigRate,
		   uint32_t Ch2pTrigRate,
		   uint32_t Ch3pTrigRate,
		   uint32_t Ch1mTrigRate,
		   uint32_t Ch2mTrigRate,
		   uint32_t Ch3mTrigRate,
		   uint32_t MaxCoarse
		   )
{
  raw_data.zero1=
    raw_data.zero2=
    raw_data.zero3=
    raw_data.zero4=
    raw_data.zero5=
    raw_data.zero6=
    raw_data.zero7=0;
    
  raw_data.IP=IP;
  raw_data.VPower1=VPower1;
  raw_data.VPower2=VPower2;
  raw_data.VPower3=VPower3;
  raw_data.VPower4=VPower4;
  raw_data.VPower5=VPower5;
  raw_data.VPower6=VPower6;

  raw_data.Th1m=Th1m;
  raw_data.Th1p=Th1p;
  raw_data.Th2m=Th2m;
  raw_data.Th2p=Th2p;
  raw_data.Th3m=Th3m;
  raw_data.Th3p=Th3p;
  raw_data.Temp=Temp;


  raw_data.TotalTrigRate=TotalTrigRate;
  raw_data.Ch1pTrigRate=Ch1pTrigRate;
  raw_data.Ch2pTrigRate=Ch2pTrigRate;
  raw_data.Ch3pTrigRate=Ch3pTrigRate;
  raw_data.Ch1mTrigRate=Ch1mTrigRate;
  raw_data.Ch2mTrigRate=Ch2mTrigRate;
  raw_data.Ch3mTrigRate=Ch3mTrigRate;

  raw_data.MaxCoarse=MaxCoarse;
}

TRENDSlc::TRENDSlc(const std::map<std::string,message_data>& md)
  :TRENDSlc(fetch_md("IP",md),
	    fetch_md("VPower1",md),
	    fetch_md("VPower2",md),
	    fetch_md("VPower3",md),
	    fetch_md("VPower4",md),
	    fetch_md("VPower5",md),
	    fetch_md("VPower6",md),
	    fetch_md("Th1-",md),fetch_md("Th1+",md),
	    fetch_md("Th2-",md),fetch_md("Th2+",md),
	    fetch_md("Th3-",md),fetch_md("Th3+",md),
	    fetch_md("Temp",md),
	    fetch_md("TotalTrigRate",md),
	    fetch_md("Ch1+TrigRate",md),
	    fetch_md("Ch2+TrigRate",md),
	    fetch_md("Ch3+TrigRate",md),
	    fetch_md("Ch1-TrigRate",md),
	    fetch_md("Ch2-TrigRate",md),
	    fetch_md("Ch3-TrigRate",md),
	    fetch_md("MaxCoarse",md)
	    )
{}

std::deque<trend_word> TRENDSlc::to_deque()const
{
  return ::to_deque(raw_data);
}

void TRENDSlc::from_deque(const std::deque<trend_word>& dq)
{
  raw_data=::from_deque<decltype(raw_data)>(dq);  
}

uint32_t TRENDSlc::type_code()const
{
  return 0x5B00;
}

double decode_vpower23(uint16_t x)
{
  return double(x)/(uint16_t(1)<<12)*6.9/2.2*5;
}

double decode_vpower1456(uint16_t x)
{
  return double(x)/(uint16_t(1)<<12)*24.0/2.0*5;
}

double decode_th(uint16_t x)
{
  return (double)x*.5;
}

double decode_temp(uint16_t x)
{
  int sign=     x&0b1000000000000;
  uint16_t x12= x&0b111111111111;

  if(sign)
    {
      x12=(~x12) &0b111111111111;
    }
  return sign?(-(x12*.0625)):(x12*.0625);
}


std::string TRENDSlc::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(use_hex)
    {
      oss<<std::hex;
    }
  oss<<"IP:"<<ip2str(raw_data.IP)<<std::endl
     <<"VPower1:"<<decode_vpower1456(raw_data.VPower1)<<std::endl
     <<"VPower2:"<<decode_vpower23(raw_data.VPower2)<<std::endl
     <<"VPower3:"<<decode_vpower23(raw_data.VPower3)<<std::endl
     <<"VPower4:"<<decode_vpower1456(raw_data.VPower4)<<std::endl
     <<"VPower5:"<<decode_vpower1456(raw_data.VPower5)<<std::endl
     <<"VPower6:"<<decode_vpower1456(raw_data.VPower6)<<std::endl;

  oss<<"[Th1-,Th1+]:"<<decode_th(raw_data.Th1m)<<" "<<decode_th(raw_data.Th1p)<<std::endl
     <<"[Th2-,Th2+]:"<<decode_th(raw_data.Th2m)<<" "<<decode_th(raw_data.Th2p)<<std::endl
     <<"[Th3-,Th3+]:"<<decode_th(raw_data.Th3m)<<" "<<decode_th(raw_data.Th3p)<<std::endl;

  oss<<"Temp:"<<decode_temp(raw_data.Temp)<<std::endl;
  oss<<"TotalTrigRate:"<<raw_data.TotalTrigRate<<std::endl;
  oss<<"Ch1+TrigRate:"<<raw_data.Ch1pTrigRate<<std::endl
     <<"Ch2+TrigRate:"<<raw_data.Ch2pTrigRate<<std::endl
     <<"Ch3+TrigRate:"<<raw_data.Ch3pTrigRate<<std::endl
     <<"Ch1-TrigRate:"<<raw_data.Ch1mTrigRate<<std::endl
     <<"Ch2-TrigRate:"<<raw_data.Ch2mTrigRate<<std::endl
     <<"Ch3-TrigRate:"<<raw_data.Ch3mTrigRate<<std::endl;
  
  oss<<"MaxCoarse:"<<raw_data.MaxCoarse<<std::endl;
  return oss.str();
}

////////////////////////
TRENDRdIntReg::TRENDRdIntReg()
{
  raw_data.zero1=
    raw_data.zero2=
    raw_data.zero3=
    raw_data.zero4=0;
  
  raw_data.IP=0x00;
  raw_data.bmac=0x00;
  raw_data.bip=0x00;

  raw_data.dmac1_lsb=0x00;
  raw_data.dmac1_msb=0x00;
  raw_data.dip1=0x00;
  
  raw_data.dmac2_lsb=0x00;
  raw_data.dmac2_msb=0x00;
  raw_data.dip2=0x00;

  raw_data.dport1=0x00;
  raw_data.dport2=0x00;

  raw_data.serial_lsb=0x00;
  raw_data.serial_msb=0x00;
}

TRENDRdIntReg::TRENDRdIntReg(uint32_t IP,
			     uint32_t bmac,
			     uint32_t bip,
			     uint64_t dmac1,
			     uint32_t dip1,
			     uint64_t dmac2,
			     uint32_t dip2,
			     uint16_t dport1,
			     uint16_t dport2,
			     uint64_t serial
			     )
{
  raw_data.zero1=
    raw_data.zero2=
    raw_data.zero3=
    raw_data.zero4=0;
  
  raw_data.IP=IP;
  raw_data.bmac=bmac;
  raw_data.bip=bip;

  raw_data.dmac1_lsb=(dmac1&0x00000000ffffffff);
  raw_data.dmac1_msb=(dmac1>>32);
  raw_data.dip1=dip1;
  
  raw_data.dmac2_lsb=(dmac2&0x00000000ffffffff);
  raw_data.dmac2_msb=(dmac2>>32);
  raw_data.dip2=dip2;

  raw_data.dport1=dport1;
  raw_data.dport2=dport2;

  raw_data.serial_lsb=(serial&0x00000000ffffffff);
  raw_data.serial_msb=(serial>>32);
}

TRENDRdIntReg::TRENDRdIntReg(const std::map<std::string,message_data>& md)
  :TRENDRdIntReg(fetch_md("IP",md),
		 fetch_md("BoardMAC",md),
		 fetch_md("BoardIP",md),
		 fetch_md("DestMAC1",md),
		 fetch_md("DestIP1",md),
		 fetch_md("DestMAC2",md),
		 fetch_md("DestIP2",md),
		 fetch_md("DestPORT1",md),
		 fetch_md("DestPORT2",md),
		 fetch_md("Serial",md))
{}

std::deque<trend_word> TRENDRdIntReg::to_deque()const
{
  return ::to_deque(raw_data);
}

void TRENDRdIntReg::from_deque(const std::deque<trend_word>& dq)
{
  raw_data=::from_deque<decltype(raw_data)>(dq);
}

uint32_t TRENDRdIntReg::type_code()const
{
  return 0x5c00;
}

std::string TRENDRdIntReg::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(use_hex)
    {
      oss<<std::hex;
    }
  oss<<"IP:"<<ip2str(raw_data.IP)<<std::endl
     <<"board mac:"<<std::setbase(16)<<raw_data.bmac<<std::endl
     <<"broad ip:"<<ip2str(raw_data.bip)<<std::endl
     <<"dest mac1:"<<std::setbase(16)<<(raw_data.dmac1_lsb+((uint64_t)raw_data.dmac1_msb<<32))<<std::endl
     <<"dest ip1:"<<ip2str(raw_data.dip1)<<std::endl
     <<"dest mac2"<<std::setbase(16)<<(raw_data.dmac2_lsb+((uint64_t)raw_data.dmac2_msb<<32))<<std::endl
     <<"dest ip2:"<<ip2str(raw_data.dip2)<<std::endl
     <<"dest port1:"<<raw_data.dport1<<std::endl
     <<"dest port2:"<<raw_data.dport2<<std::endl
     <<"serial:"<<(raw_data.serial_lsb+((uint64_t)raw_data.serial_msb<<32));  
  
  return oss.str();
}

////////////////////////
TRENDADC::TRENDADC()
  :data(0x00)
{

}

TRENDADC::TRENDADC(uint8_t addr,
		   uint16_t reg_func)
{
  data=((uint32_t)(addr&0x1f)<<11)+(reg_func&0x7ff);
}

TRENDADC::TRENDADC(const std::map<std::string,message_data>& md)
  :TRENDADC(fetch_md("ADDR",md),
	    fetch_md("REGFUNC",md))
{}

std::deque<trend_word> TRENDADC::to_deque()const
{
  return ::to_deque(data);
}

void TRENDADC::from_deque(const std::deque<trend_word> & dq)
{
  data=::from_deque<decltype(data)>(dq);
}

uint32_t TRENDADC::type_code()const
{
  return 0x5400;
}

std::string TRENDADC::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(use_hex)
    {
      oss<<std::hex;
    }
  oss<<"ADDR:"<<(data>>11)<<std::endl
     <<"REG FUNC:"<<(data&0x7ff);
  return oss.str();
}
////////////////////////
TRENDACK::TRENDACK()
{
  raw_data.IP=0x00;
  raw_data.MsgAck=0x00;
}



TRENDACK::TRENDACK(uint32_t IP,
		   uint32_t MsgAck)
{
  raw_data.IP=IP;
  raw_data.MsgAck=MsgAck;
}

TRENDACK::TRENDACK(const std::map<std::string,message_data>& md)
  :TRENDACK(fetch_md("IP",md),
	    fetch_md("MsgAck",md))
{}

std::deque<trend_word> TRENDACK::to_deque()const
{
  return ::to_deque(raw_data);
}

void TRENDACK::from_deque(const std::deque<trend_word> & dq)
{
  raw_data=::from_deque<decltype(raw_data)>(dq);
}

uint32_t TRENDACK::type_code()const
{
  return 0x5d00;
}

std::string TRENDACK::to_string(bool use_hex)const
{
  std::ostringstream oss;
  if(use_hex)
    {
      oss<<std::hex;
    }
  oss<<"IP:"<<ip2str(raw_data.IP)<<std::endl
     <<"MSGACK:"<<raw_data.MsgAck;
  return oss.str();
}
