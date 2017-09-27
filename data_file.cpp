#include "data_file.hpp"
#include <fstream>

file_header_t::file_header_t () : basic_header{}, additional_header{ 0 }
{
    basic_header.length = sizeof (basic_header);
}

file_header_t::file_header_t (const basic_file_header_t &bfh, const std::vector<std::uint32_t> &ah)
: basic_header (bfh), additional_header (ah)
{
    int additional_length = additional_header.size ();
    basic_header.length = (additional_length - 1) * sizeof (std::uint32_t) + sizeof (basic_header);
}

read_result file_header_t::read_from (std::istream &is)
{
    is.read ((char *)&basic_header, sizeof (basic_header));
    if(!is.good())
    {
        return read_result::FAILED;
    }
    int additional_length = (basic_header.length - sizeof (basic_header)) / sizeof (std::int32_t) + 1;
    additional_header.resize (additional_length);
    
    is.read ((char *)additional_header.data (), additional_length * sizeof (std::int32_t));
    if(!is.good())
    {
        return read_result::FAILED;
    }
    return read_result::OK;
}

write_result file_header_t::write_to (std::ostream &os) const
{
    os.write ((char *)&basic_header, sizeof (basic_header));
    os.write ((char *)additional_header.data (), additional_header.size () * sizeof (std::int32_t));
    return write_result::OK;
}

std::size_t file_header_t::size () const
{
    return sizeof (basic_header) + additional_header.size () * sizeof (std::uint32_t);
}

std::int32_t file_header_t::length()const{return basic_header.length;}
std::int32_t file_header_t::runnr()const{return basic_header.runnr;}
std::int32_t file_header_t::run_mode()const{return basic_header.run_mode;}
std::int32_t file_header_t::serial()const{return basic_header.serial;}
std::int32_t file_header_t::first_event()const{return basic_header.first_event;}
std::int32_t file_header_t::first_event_sec()const{return basic_header.first_event_sec;}
std::int32_t file_header_t::last_event()const{return basic_header.last_event;}
std::int32_t file_header_t::last_event_sec()const{return basic_header.last_event_sec;}

std::vector<std::uint32_t> file_header_t::get_additional_header()const
{
    return additional_header;
}

std::ostream &operator<< (std::ostream &os, const basic_file_header_t &bfh)
{
    os << "file header length=" << bfh.length << std::endl;
    os << "runnr=" << bfh.runnr << std::endl;
    os << "run_mode" << bfh.run_mode << std::endl;
    os << "serial=" << bfh.serial << std::endl;
    os << "first event=" << bfh.first_event << std::endl;
    os << "first_event sec=" << bfh.first_event_sec << std::endl;
    os << "last event=" << bfh.last_event << std::endl;
    os << "last event sec =" << bfh.last_event_sec << std::endl;
    return os;
}

std::ostream &operator<< (std::ostream &os, const file_header_t &fh)
{
    os << fh.basic_header;
    for (std::size_t i = 0; i < fh.additional_header.size (); ++i)
        {
            os << "Additional header word " << i << " = " << fh.additional_header[i] << std::endl;
        }
    return os;
}

std::ostream &operator<< (std::ostream &os, const event_header_t &eh)
{
    os << "Header length = " << eh.header_length << std::endl
       << "Runnr = " << eh.runnr << std::endl
       << "Event nr = " << eh.eventnr << std::endl
       << "T3Event nr = " << eh.t3eventnr << std::endl
       << "First LS = " << eh.first_ls << std::endl
       << "Event sec = " << eh.event_sec << std::endl
       << "Event nsec = " << eh.event_nsec << std::endl
       << "Event type = " << std::hex << "0x" << eh.event_type << std::endl
       << "Event version = " << std::dec << eh.event_vers << std::endl
       << "AD1 = " << eh.ad1 << std::endl
       << "AD2 = " << eh.ad2 << std::endl
       << "LSCNT = " << eh.ls_cnt << std::endl;
    return os;
}

bool event_header_t::operator==(const event_header_t& rhs)const
{
    return header_length==rhs.header_length && 
    runnr==rhs.runnr && 
    eventnr==rhs.eventnr && 
    t3eventnr==rhs.t3eventnr && 
    first_ls==rhs.first_ls && 
    event_sec==rhs.event_sec && 
    event_nsec==rhs.event_nsec && 
    event_type==rhs.event_type && 
    event_vers==rhs.event_vers && 
    ad1==rhs.ad1 && 
    ad2==rhs.ad2 && 
    ls_cnt==rhs.ls_cnt;
}

std::ostream &operator<< (std::ostream &os, const local_station_header_t &lsh)
{
    os << "Length = " << lsh.length << std::endl
       << "Event nr = " << lsh.event_nr << std::endl
       << "LS_id = " << lsh.ls_id << std::endl
       << "header length = " << lsh.header_length << std::endl
       << "gps seconds = " << lsh.GPSseconds << std::endl
       << "gps nanosec = " << lsh.GPSnanoseconds << std::endl
       << "trigger flag = " << lsh.trigger_flag << std::endl
       << "trigger pos = " << lsh.trigger_pos << std::endl
       << "sampling freq = " << lsh.sampling_freq << std::endl
       << "channel_mask =" << lsh.channel_mask << std::endl
       << "adc resolution = " << lsh.ADC_resolution << std::endl
       << "trace length = " << lsh.trace_length << std::endl
       << "version = " << lsh.version << std::endl;
    return os;
}

bool local_station_header_t::operator==(const local_station_header_t& rhs)const
{
    return length==rhs.length &&
    event_nr==rhs.event_nr &&
    ls_id==rhs.ls_id &&
    header_length==rhs.header_length &&
    GPSseconds==rhs.GPSseconds &&
    GPSnanoseconds==rhs.GPSnanoseconds &&
    trigger_flag==rhs.trigger_flag &&
    trigger_pos==rhs.trigger_pos &&
    sampling_freq==rhs.sampling_freq &&
    channel_mask==rhs.channel_mask &&
    ADC_resolution==rhs.ADC_resolution &&
    trace_length==rhs.trace_length &&
    version==rhs.version;
}

local_station_t::local_station_t (const local_station_header_t &lsh,
                                  const std::vector<std::uint16_t> &hd,
                                  const std::vector<std::uint16_t> &ab)
: header (lsh), header_data (hd), adc_buffer (ab)
{
    header.length = size () / 2;
    header.header_length = hd.size () + 13;
    header.trace_length = ab.size ();
}

bool local_station_t::operator==(const local_station_t& rhs)const
{
    return rhs.header==this->header &&rhs.header_data==this->header_data&&rhs.adc_buffer==this->adc_buffer;
}

read_result local_station_t::read_from (std::istream &is)
{
    is.read ((char *)&header, sizeof (header));
    if(!is.good())
    {
        return read_result::FAILED;
    }
    int header_length = header.header_length - 13; // 13 is a magic number
    int data_length = header.trace_length;
    header_data.resize (header_length);
    adc_buffer.resize (data_length);
    
    is.read ((char *)header_data.data (), sizeof (std::uint16_t) * header_data.size ());
    if(!is.good())
    {
        return read_result::FAILED;
    }
    
    is.read ((char *)adc_buffer.data (), sizeof (std::uint16_t) * adc_buffer.size ());
    if(!is.good())
    {
        return read_result::FAILED;
    }
    return read_result::OK;
}

write_result local_station_t::write_to (std::ostream &os) const
{
    os.write ((char *)&header, sizeof (header));
    //std::cerr<<header_data.size()<<std::endl;
    os.write ((char *)header_data.data (), sizeof (std::uint16_t) * header_data.size ());
    os.write ((char *)adc_buffer.data (), sizeof (std::uint16_t) * adc_buffer.size ());
    return write_result::OK;
}

std::size_t local_station_t::size () const
{
    return sizeof (header) + header_data.size () * sizeof (std::uint16_t) +
           adc_buffer.size () * sizeof (std::uint16_t);
}

std::ostream &operator<< (std::ostream &os, const local_station_t &ls)
{
    os << ls.header << std::endl;
    os << "Header data=[" << ls.header_data.size () << " ] ";
    for (auto &x : ls.header_data)
        {
            os << (short)x << " ";
        }
    os << std::endl;
    os << "DATA = [" << ls.adc_buffer.size () << " ]";
    for (auto &x : ls.adc_buffer)
        {
            os << (short)x << " ";
        }
    os << std::endl;
    return os;
}

event_t::event_t (const event_header_t &h) : header (h)
{
    header.header_length = size () - 3;
}

void event_t::append_local_station (const local_station_t &ls)
{
    local_station_list.push_back (ls);
    header.header_length = size () - 4;
}

bool event_t::operator==(const event_t& rhs)const
{
    return header==rhs.header && local_station_list==rhs.local_station_list;
}

read_result event_t::read_from (std::istream &is)
{
    is.read ((char *)&header, sizeof (header));
    if(!is.good())
    {
        return read_result::FAILED;
    }
    int nls = header.ls_cnt;
    for (int i = 0; i < nls; ++i)
        {
            local_station_t ls;
            if(ls.read_from (is)!=read_result::OK)
            {
                return read_result::FAILED;
            }
            local_station_list.push_back (ls);
        }
    return read_result::OK;
}

write_result event_t::write_to (std::ostream &os) const
{
    os.write ((char *)&header, sizeof (header));
    int nls = header.ls_cnt;
    for (int i = 0; i < nls; ++i)
        {
            if(local_station_list[i].write_to (os)!=write_result::OK)
            {
                return write_result::FAILED;
            }
        }
    return write_result::OK;
}

std::size_t event_t::size () const
{
    std::size_t result = sizeof (header);
    for (auto &x : local_station_list)
        {
            result += x.size ();
        }
    return result;
}

std::ostream &operator<< (std::ostream &os, const event_t &e)
{
    os << e.header << std::endl;
    for (auto &x : e.local_station_list)
        {
            os << x << std::endl;
        }
    return os;
}

event_file::event_file(std::istream& is)
{
    read_from(is);
}

read_result event_file::read_from(std::istream& is)
{
    if(header.read_from(is)==read_result::OK)
    {
        for(;;)
        {
            auto ev=read_event(is);
            if(ev)
            {
                event_list.push_back(ev.value());
            }
            else
            {
                break;
            }
        }
    }
    return read_result::OK;
}

write_result event_file::write_to(std::ostream& os)const
{
    if(header.write_to(os)==write_result::FAILED)
    {
        return write_result::FAILED;
    }
    for(const auto& i:event_list)
    {
        if(i.write_to(os)!=write_result::OK)
        {
            return write_result::FAILED;
        }
    }
    return write_result::OK;
}

std::optional<file_header_t> read_file_header(std::istream& is)
{
    file_header_t fh;
    if(fh.read_from(is)==read_result::OK)
    {
        return fh;
    }
    else
    {
        return {};
    }
}

std::optional<event_t> read_event(std::istream& is)
{
    event_t ev;
    if(ev.read_from(is)==read_result::OK)
    {
        return ev;
    }
    else
    {
        return {};
    }
}

event_file read_event_file(const std::string& fname)
{
    event_file ef;
    std::ifstream ifs(fname.c_str());
    ef.read_from(ifs);
    return ef;
}
