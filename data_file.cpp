#include "data_file.hpp"


file_header_t::file_header_t(const basic_file_header_t& bfh, const std::vector<std::uint32_t>& ah)
:basic_header(bfh), additional_header(ah)
{
    int additional_length=additional_header.size();
    basic_header.length=(additional_length-1)*sizeof(std::uint32_t)+sizeof(basic_header);
}

void file_header_t::read_from(std::istream& is)
{
    is.read((char*)&basic_header, sizeof(basic_header));
    int additional_length=(basic_header.length-sizeof(basic_header))/sizeof(std::int32_t)+1;
    additional_header.resize(additional_length);
    is.read((char*)additional_header.data(), additional_length*sizeof(std::int32_t));
}

void file_header_t::write_to(std::ostream& os)const
{
    os.write((char*)&basic_header, sizeof(basic_header));
    os.write((char*)additional_header.data(), additional_header.size()*sizeof(std::int32_t));
}

std::size_t file_header_t::size()const
{
    return sizeof(basic_header)+additional_header.size()*sizeof(std::uint32_t);
}

std::ostream& operator<<(std::ostream& os, const basic_file_header_t& bfh)
{
    os<<"file header length="<<bfh.length<<std::endl;
    os<<"runnr="<<bfh.runnr<<std::endl;
    os<<"run_mode"<<bfh.run_mode<<std::endl;
    os<<"serial="<<bfh.serial<<std::endl;
    os<<"first event="<<bfh.first_event<<std::endl;
    os<<"first_event sec="<<bfh.first_event_sec<<std::endl;
    os<<"last event="<<bfh.last_event<<std::endl;
    os<<"last event sec ="<<bfh.last_event_sec<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const file_header_t& fh)
{
    os<<fh.basic_header;
    for(std::size_t i=0;i<fh.additional_header.size();++i)
    {
        os<<"Additional header word "<<i<<" = "<<fh.additional_header[i]<<std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const event_header_t& eh)
{
    os  <<"Header length = "    <<eh.header_length<<std::endl
        <<"Runnr = "            <<eh.runnr<<std::endl
        <<"Event nr = "         <<eh.eventnr<<std::endl
        <<"T3Event nr = "       <<eh.t3eventnr<<std::endl
        <<"First LS = "         <<eh.first_ls<<std::endl
        <<"Event sec = "        <<eh.event_sec<<std::endl
        <<"Event nsec = "       <<eh.event_nsec<<std::endl
        <<"Event type = "<<std::hex<<"0x"       <<eh.event_type<<std::endl
        <<"Event version = "<<std::dec    <<eh.event_vers<<std::endl
        <<"AD1 = "              <<eh.ad1<<std::endl
        <<"AD2 = "              <<eh.ad2<<std::endl
        <<"LSCNT = "            <<eh.ls_cnt<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const local_station_header_t& lsh)
{
    os  <<"Length = "       <<lsh.length<<std::endl
        <<"Event nr = "     <<lsh.event_nr<<std::endl
        <<"LS_id = "        <<lsh.ls_id<<std::endl
        <<"header length = "<<lsh.header_length<<std::endl
        <<"gps seconds = "  <<lsh.GPSseconds<<std::endl
        <<"gps nanosec = "  <<lsh.GPSnanoseconds<<std::endl
        <<"trigger flag = " <<lsh.trigger_flag<<std::endl
        <<"trigger pos = "  <<lsh.trigger_pos<<std::endl
        <<"sampling freq = "<<lsh.sampling_freq<<std::endl
        <<"channel_mask ="  <<lsh.channel_mask<<std::endl
        <<"adc resolution = "<<lsh.ADC_resolution<<std::endl
        <<"trace length = " <<lsh.trace_length<<std::endl
        <<"version = "      <<lsh.version<<std::endl;
    return os;
}

local_station_t::local_station_t(const local_station_header_t& lsh, 
const std::vector<std::uint16_t>& hd, const std::vector<std::uint16_t>& ab)
:header(lsh), header_data(hd), adc_buffer(ab)
{
    header.length=size()/2;
    header.header_length=hd.size()+13;
}

void local_station_t::read_from(std::istream& is)
{
    is.read((char*)&header, sizeof(header));
    int header_length=header.header_length-13;//13 is a magic number
    int data_length=header.trace_length;
    header_data.resize(header_length);
    adc_buffer.resize(data_length);
    is.read((char*)header_data.data(), sizeof(std::uint16_t)*header_data.size());
    is.read((char*)adc_buffer.data(), sizeof(std::uint16_t)*adc_buffer.size());
}

void local_station_t::write_to(std::ostream& os)const
{
    os.write((char*)&header, sizeof(header));
    os.write((char*)header_data.data(), sizeof(std::uint16_t)*header_data.size());
    os.write((char*)adc_buffer.data(), sizeof(std::uint16_t)*adc_buffer.size());
}

std::size_t local_station_t::size()const
{
    return sizeof(header)+header_data.size()*sizeof(std::uint16_t)+adc_buffer.size()*sizeof(std::uint16_t);
}

std::ostream& operator<<(std::ostream& os, const local_station_t& ls)
{
    os<<ls.header<<std::endl;
    os<<"Header data=["<<ls.header_data.size()<<" ] ";
    for(auto& x:ls.header_data)
    {
        os<<(short)x<<" ";
    }
    os<<std::endl;
    os<<"DATA = ["<<ls.adc_buffer.size()<<" ]";
    for(auto& x:ls.adc_buffer)
    {
        os<<(short)x<<" ";
    }
    os<<std::endl;
    return os;
}

event_t::event_t(const event_header_t& h)
:header(h)
{
    header.header_length=size()-3;
}

void event_t::append_local_station(const local_station_t& ls)
{
    local_station_list.push_back(ls);
    header.header_length=size()-4;
}

void event_t::read_from(std::istream& is)
{
    is.read((char*)&header, sizeof(header));
    int nls=header.ls_cnt;
    for(int i=0;i<nls;++i)
    {
        local_station_t ls;
        ls.read_from(is);
        local_station_list.push_back(ls);
    }
}

void event_t::write_to(std::ostream& os)const
{
    os.write((char*)&header, sizeof(header));
    int nls=header.ls_cnt;
    for(int i=0;i<nls;++i)
    {
        local_station_list[i].write_to(os);
    }
}

std::size_t event_t::size()const
{
    std::size_t result=sizeof(header);
    for(auto& x:local_station_list)
    {
        result+=x.size();
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const event_t& e)
{
    os<<e.header<<std::endl;
    for(auto& x:e.local_station_list)
    {
        os<<x<<std::endl;
    }
    return os;
}
