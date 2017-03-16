#ifndef TREND_MESSAGE
#define TREND_MESSAGE
#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <memory>
#include <cassert>
#include <cstdint>
struct trend_word
{
  uint32_t data;
  trend_word();

  trend_word(uint32_t n);

  trend_word(const unsigned char* p);
  
  trend_word switch_endian()const;

  bool operator==(const trend_word& rhs)const;
  bool operator==(const uint32_t& rhs)const;
  bool is_header_or_trailer()const;
  operator uint32_t()const;  
};

std::ostream& operator<<(std::ostream& os,const trend_word& tw);

enum class message_type
{
  TRENDDAQ,TRENDTRIG,TRENDSlcReq,TRENDGPS,TRENDIntReg,TRENDData,TRENDSlc,TRENDRdIntReg,TRENDACK,UNKNOWN,NOTAMESSAGE
};

struct message_data
{
  uint64_t scalar_value;
  std::vector<uint64_t> vector_value;

  template <typename T>
  message_data(T v)
    :scalar_value(static_cast<uint64_t>(v))
  {}
		 
  message_data(std::initializer_list<uint64_t> v)
    :scalar_value(0),vector_value(v)
  {}

  template <typename T>
  message_data(const std::vector<T>& v)
    :scalar_value(0)
  {
    for(size_t i=0;i<v.size();++i)
      {
	vector_value.push_back(static_cast<uint64_t>(v[i]));
      }
  }

  message_data(const message_data&)=default;
  message_data& operator=(const message_data&)=default;

  message_data();

  template <typename T>
  std::vector<T> fetch_vector()const
  {
    std::vector<T> result(vector_value.size());
    for(size_t i=0;i<vector_value.size();++i)
      {
	result[i]=vector_value[i];
      }
    return result;
  }

  template <typename T>
  operator std::vector<T>()const
  {
    return fetch_vector<T>();
  }

  template <typename T>
  T fetch_scalar()const
  {
    return static_cast<T>(scalar_value);
  }

  template <typename T>
  operator T()const
  {
    return fetch_scalar<T>();
  }
};

struct base_message
{
  bool valid;
  virtual ~base_message();
    
  virtual std::deque<trend_word> to_deque()const=0;
  virtual void from_deque(const std::deque<trend_word>& dq)=0;
  virtual uint32_t type_code()const=0;
  virtual std::string to_string(bool use_hex)const=0;
};


std::shared_ptr<base_message> create_message(std::string message_type,
					     const std::map<std::string,message_data>& data);

template <typename T>
std::deque<trend_word> to_deque(const T& x)
{
  std::deque<trend_word> result;
  static_assert(sizeof(T)%4==0,"Error, the size of type T must be 4x");
  const uint32_t* data=reinterpret_cast<const uint32_t*>(&x);
  
  for(int i=0;i<sizeof(T)/4;++i)
    {
      result.push_back(data[i]);
    }
  return result;
}

template<typename T>
T from_deque(const std::deque<trend_word>& dq)
{
  static_assert(sizeof(T)%4==0,"Error, the size of type T must be 4x");
  //std::cerr<<sizeof(T);
  assert(sizeof(T)/4<=dq.size());
  uint32_t data[sizeof(T)/4];
  for(int i=0;i<(sizeof(T)/4);++i)
    {
      data[i]=dq[i];
    }
  return T(*(reinterpret_cast<T*>(data)));
}

struct TRENDDAQ
  :public base_message
{
  struct
  {
    unsigned int daq_on:1;
    unsigned int ant_on:3;
    unsigned int rd_wr_plus:1;
    unsigned int en_osc:1;
    unsigned int CntrlADC:2;
    unsigned int offst:12;
    unsigned int enable_pd:3;
    unsigned int dis_lna:3;
    unsigned int le:6;
    unsigned int att1:7;
    unsigned int att2:7;
    unsigned int zero:18;
  }raw_data;

  TRENDDAQ();

  TRENDDAQ(const std::map<std::string,message_data>& md);

  TRENDDAQ(bool daq_on,
	   uint8_t ant_on,
	   bool rd_wr_plus,
	   bool en_osc,
	   uint8_t CntrlADC,
	   uint16_t offst,
	   uint8_t enable_pd,
	   uint8_t dis_lna,
	   uint8_t le,
	   uint8_t att1,
	   uint8_t att2);
	   
  TRENDDAQ(const TRENDDAQ&)=default;
  TRENDDAQ& operator=(const TRENDDAQ&)=default;
  
  std::deque<trend_word> to_deque()const;

  void from_deque(const std::deque<trend_word>& dq);

  uint32_t type_code()const;

  std::string to_string(bool use_hex)const;
};

struct TRENDTRIG
  :public base_message
{
  struct
  {
    unsigned int ST:1;
    unsigned int TrgEn:6;
    unsigned int :1;
    unsigned int CntrlDAC:16;
    unsigned int :8;
    unsigned int Th1m:12;
    unsigned int Th1p:12;
    unsigned int :8;
    unsigned int Th2m:12;
    unsigned int Th2p:12;
    unsigned int :8;
    unsigned int Th3m:12;
    unsigned int Th3p:12;
    unsigned int :8;
  }raw_data;
  
  TRENDTRIG()=default;
  
  TRENDTRIG(uint8_t ST,
	    uint8_t TrgEn,
	    uint16_t CntrlDAC,
	    uint16_t th1m,uint16_t th1p,
	    uint16_t th2m,uint16_t th2p,
	    uint16_t th3m,uint16_t th3p
	    );

  TRENDTRIG(const std::map<std::string,message_data>& md);
  
  TRENDTRIG(const TRENDTRIG& rhs);
  TRENDTRIG& operator=(const TRENDTRIG& rhs);
  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};

struct TRENDSlcReq
  :public base_message
{
  TRENDSlcReq()=default;
  TRENDSlcReq(const std::map<std::string,message_data>& md);
  
  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};

struct TRENDGPS
  :public base_message
{
  struct{
    unsigned int RWb:1;
    unsigned int Addr:7;
    unsigned int wrd:6;
    unsigned int zeros:18;
  }header;

  std::vector<uint8_t> data;
  
  TRENDGPS();
  TRENDGPS(uint8_t RWb,uint8_t Addr,uint8_t wrd,const std::vector<uint8_t> d);
  TRENDGPS(const std::map<std::string,message_data>& md);
  TRENDGPS(const TRENDGPS&)=default;
  TRENDGPS& operator=(const TRENDGPS&)=default;
  
  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};


struct TRENDIntReg
  :public base_message
{
  struct
  {
    uint32_t rw;
    uint32_t AD0;
    uint32_t AD1;
    uint32_t AD2;
    uint32_t AD3;
    uint32_t AD4;
    uint32_t AD5;
    uint32_t AD6;
    uint32_t AD7;
    uint32_t AD8;
    uint32_t AD9;
  }raw_data;
  
  TRENDIntReg()=default;
  
  TRENDIntReg(bool write,
	      uint32_t bmac,
	      uint32_t bip,
	      uint64_t dmac1,
	      uint32_t dip1,
	      uint64_t dmac2,
	      uint32_t dip2,
	      uint16_t port1,
	      uint16_t port2);

  TRENDIntReg(const std::map<std::string,message_data>& md);
  
  TRENDIntReg(const TRENDIntReg& rhs);
  TRENDIntReg& operator=(const TRENDIntReg& rhs);
  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};


struct TRENDData
  :public base_message
{
  struct
  {
    uint32_t IP;
    
    uint32_t TS2;
    
    unsigned int TS1Trigger:8;
    unsigned int TS1PPS:8;
    unsigned int SSS:16;
    
    uint32_t EC;
    
    unsigned int TrigPattern:6;
    unsigned int zero:26;
  }header;

  std::vector<uint16_t> data;
  
  TRENDData();
  
  TRENDData(uint32_t IP,
	    uint32_t TS2,
	    uint8_t TS1Trigger,
	    uint8_t TS1PPS,
	    uint16_t SSS,
	    uint32_t EC,
	    uint8_t TrigPattern,
	    const std::vector<uint16_t>& d
	    );

  TRENDData(const std::map<std::string,message_data>& md);
  
  TRENDData(const TRENDData& rhs)=default;
  TRENDData& operator=(const TRENDData& rhs)=default;
  
  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};

struct TRENDSlc
  :public base_message
{
  struct
  {
    uint32_t IP;
    
    unsigned int VPower1:12;
    unsigned int VPower2:12;
    unsigned int zero1:8;

    unsigned int VPower3:12;
    unsigned int VPower4:12;
    unsigned int zero2:8;

    unsigned int VPower5:12;
    unsigned int VPower6:12;
    unsigned int zero3:8;

    unsigned int Th1m:12;
    unsigned int Th1p:12;
    unsigned int zero4:8;

    unsigned int Th2m:12;
    unsigned int Th2p:12;
    unsigned int zero5:8;

    unsigned int Th3m:12;
    unsigned int Th3p:12;
    unsigned int zero6:8;

    unsigned int Temp:13;
    unsigned int zero7:19;

    uint32_t TotalTrigRate;

    uint32_t Ch1pTrigRate;

    uint32_t Ch2pTrigRate;

    uint32_t Ch3pTrigRate;
    
    uint32_t Ch1mTrigRate;

    uint32_t Ch2mTrigRate;

    uint32_t Ch3mTrigRate;

    uint32_t MaxCoarse;

  }raw_data;

  
  TRENDSlc();
  
  TRENDSlc(uint32_t IP,
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
	   );

  TRENDSlc(const std::map<std::string,message_data>& md);
  TRENDSlc(const TRENDSlc& rhs)=default;
  TRENDSlc& operator=(const TRENDSlc& rhs)=default;
  
  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};


struct TRENDRdIntReg
  :public base_message
{
  struct
  {
    uint32_t IP;
    uint32_t bmac;
    uint32_t bip;
    uint32_t dmac1_lsb;
    
    unsigned int dmac1_msb:16;
    unsigned int zero1:16;
    
    uint32_t dip1;
    
    uint32_t dmac2_lsb;
    unsigned int dmac2_msb:16;
    unsigned int zero2:16;
    
    uint32_t dip2;
    
    unsigned int dport1:16;
    unsigned int zero3:16;
    
    unsigned int dport2:16;
    unsigned int zero4:16;
    
    uint32_t serial_lsb;
    uint32_t serial_msb;
  }raw_data;

  
  TRENDRdIntReg();
  
  TRENDRdIntReg(uint32_t IP,
		uint32_t bmac,
		uint32_t bip,
		uint64_t dmac1,
		uint32_t dip1,
		uint64_t dmac2,
		uint32_t dip2,
		uint16_t dport1,
		uint16_t dport2,
		uint64_t serial
		);
  
  TRENDRdIntReg(const std::map<std::string,message_data>& md);
  
  TRENDRdIntReg(const TRENDRdIntReg& rhs)=default;
  TRENDRdIntReg& operator=(const TRENDRdIntReg& rhs)=default;
  
  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};

struct TRENDADC
  :public base_message
{
  uint32_t data;

  TRENDADC();
  TRENDADC(uint8_t addr,uint16_t reg_func);
  TRENDADC(const std::map<std::string,message_data>& md);
  
  TRENDADC(const TRENDADC&)=default;
  TRENDADC& operator=(const TRENDADC&)=default;

  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};

struct TRENDACK
  :public base_message
{
  struct
  {
    uint32_t IP;
    uint32_t MsgAck;
  }raw_data;

  TRENDACK();
  TRENDACK(uint32_t IP,
	   uint32_t MsgAck);
  TRENDACK(const std::map<std::string,message_data>& md);
  TRENDACK(const TRENDACK&)=default;
  TRENDACK& operator=(const TRENDACK&)=default;
    
  std::deque<trend_word> to_deque()const;
  void from_deque(const std::deque<trend_word>& dq);
  uint32_t type_code()const;
  std::string to_string(bool use_hex)const;
};

#endif
