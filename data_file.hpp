#ifndef DATA_FILE_HPP
#define DATA_FILE_HPP
#include <cstdint>
#include <iostream>
#include <vector>
#if __cplusplus >=201703L
#include <optional>
#else

namespace std
{
    template <typename T>
    class optional
    {
    private:
        T _value;
        bool _value_set;
    public:
        optional(const T& rhs)
        :_value(rhs),_value_set(true)
        {}

        optional(optional<T>&& rhs)=default;
        optional(const optional<T>& rhs)=default;
        optional()
        :_value(), _value_set(false)
        {}

        optional<T>& operator=(const optional<T>& rhs)=default;
        bool operator==(const optional<T>& rhs)const
        {
            if(!_value_set&&rhs._value_set)
            {
                return true;
            }
            else
            {
                return _value_set==rhs._value_set&&_value==rhs._value;
            }
        }
        operator bool()const
        {
            return _value_set;
        }
    public:
        T& value()
        {
            return _value;
        }

        const T& value()const
        {
            return _value;
        }
    };
}


#endif
enum class write_result
{
    OK,FAILED
};

enum class read_result
{
    OK, FAILED
};

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

    file_header_t ();
    file_header_t (const file_header_t &) = default;
    file_header_t &operator= (const file_header_t &) = default;
    file_header_t (const basic_file_header_t &bfh, const std::vector<std::uint32_t> &ah);

    read_result read_from (std::istream &is);
    write_result write_to (std::ostream &os) const;
    std::size_t size () const;

    std::int32_t length()const;
    std::int32_t runnr()const;
    std::int32_t run_mode()const;
    std::int32_t serial()const;
    std::int32_t first_event()const;
    std::int32_t first_event_sec()const;
    std::int32_t last_event()const;
    std::int32_t last_event_sec()const;
    std::vector<std::uint32_t> get_additional_header()const;
};

std::ostream &operator<< (std::ostream &os, const basic_file_header_t &bfh);
std::ostream &operator<< (std::ostream &os, const file_header_t &fh);

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
    bool operator==(const event_header_t& rhs)const;
};

std::ostream &operator<< (std::ostream &os, const event_header_t &eh);

struct local_station_header_t
{
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
    std::uint16_t trace_length;
    std::uint16_t version;

    bool operator==(const local_station_header_t&)const;
};

std::ostream &operator<< (std::ostream &os, const local_station_header_t &lsh);

struct local_station_t
{
    local_station_header_t header;
    std::vector<std::uint16_t> header_data;
    std::vector<std::uint16_t> adc_buffer;

    local_station_t () = default;
    local_station_t (const local_station_t &) = default;
    local_station_t &operator= (const local_station_t &) = default;
    local_station_t (const local_station_header_t &lsh,
                     const std::vector<std::uint16_t> &hd,
                     const std::vector<std::uint16_t> &ab);
    bool operator==(const local_station_t&)const;
    read_result read_from (std::istream &is);
    write_result write_to (std::ostream &os) const;
    std::size_t size () const;
};

std::ostream &operator<< (std::ostream &os, const local_station_t &ls);

struct event_t
{
    event_header_t header;
    std::vector<local_station_t> local_station_list;

    event_t () = default;
    event_t (const event_t &) = default;
    event_t &operator= (const event_t &) = default;
    event_t (const event_header_t &h);

    void append_local_station (const local_station_t &ls);
    read_result read_from (std::istream &is);
    write_result write_to (std::ostream &os) const;
    std::size_t size () const;
    bool operator==(const event_t& rhs)const;
};

struct event_file
{
    file_header_t header;
    std::vector<event_t> event_list;
    event_file(std::istream& is);
    event_file()=default;
    event_file(const event_file&)=default;
    ~event_file()=default;
    event_file& operator=(const event_file&)=default;

    read_result read_from(std::istream& is);
    write_result write_to(std::ostream& os)const;
};

std::ostream &operator<< (std::ostream &os, const event_t &e);

std::optional<file_header_t> read_file_header(std::istream& is);
std::optional<event_t> read_event(std::istream& is);

event_file read_event_file(const std::string& fname);

#endif
