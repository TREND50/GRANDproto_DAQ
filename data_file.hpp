#ifndef DATA_FILE_HPP
#define DATA_FILE_HPP
#include <cstdint>
#include <iostream>
#include <vector>

struct basic_file_header_t
{
    std::int32_t length;
    std::int32_t runnr;
    std::int32_t run_mode; 
    std::int32_t serial;
    std::int32_t first_event;
    std::int32_t first_event_sec;
    std::int32_t last_event;
    std::int32_t last_event_sec;
};

struct file_header_t
{
    basic_file_header_t basic_header;
    std::vector<std::uint32_t> additional_header;

    void read_from(std::istream& is);
    std::size_t size()const;
};

std::ostream& operator<<(std::ostream& os, const basic_file_header_t& bfh);
std::ostream& operator<<(std::ostream& os, const file_header_t& fh);

struct event_header_t
{
    std::int32_t header_length;
    std::int32_t runnr;
    std::int32_t eventnr;
    std::int32_t t3eventnr;
    std::int32_t first_ls;
    std::int32_t event_sec;
    std::int32_t event_nsec;
    std::int16_t event_type;
    std::int16_t event_vers;
    std::int32_t ad1;
    std::int32_t ad2;
    std::int32_t ls_cnt;
};

std::ostream& operator<<(std::ostream& os, const event_header_t& eh);

struct local_station_header_t{
    std::uint16_t length;
    std::uint16_t event_nr;
    std::uint16_t ls_id;
    std::uint16_t header_length;
    std::uint32_t GPSseconds;
    std::uint32_t GPSnanoseconds;
    std::uint16_t trigger_flag;
    std::uint16_t trigger_pos;
    std::uint16_t sampling_freq;
    std::uint16_t channel_mask;
    std::uint16_t ADC_resolution;
    std::uint16_t tracelength;
    std::uint16_t version;
  };

std::ostream& operator<<(std::ostream& os, const local_station_header_t& lsh);
  
struct local_station_t
{
    local_station_header_t header;
    std::vector<std::uint16_t> header_data;
    std::vector<std::uint16_t> adc_buffer;

    void read_from(std::istream& is);
    std::size_t size()const;
};

std::ostream& operator<<(std::ostream& os, const local_station_t& ls);

struct event_t
{
    event_header_t header;
    std::vector<local_station_t> local_station_list;

    void read_from(std::istream& is);
    std::size_t size()const;
};

std::ostream& operator<<(std::ostream& os, const event_t& e);

#endif
