#include <fstream>
#include <iostream>

#include "data_file.hpp"

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        std::cerr<<"Usage: "<<argv[0]<<" <bin file>"<<std::endl;
        return -1;
    }
    std::ifstream ifs(argv[1]);
    file_header_t fh;
    fh.read_from(ifs);
    std::cout<<fh<<std::endl;

    
    event_t ev;
    ev.read_from(ifs);
    std::cout<<ev<<std::endl;
    std::cout<<ev.size()<<std::endl;
    std::cout<<ev.local_station_list[0].size()<<std::endl;
    std::ofstream ofs("dump.bin");
    fh.write_to(ofs);
    local_station_t ls(ev.local_station_list[0].header,ev.local_station_list[0].header_data, ev.local_station_list[0].adc_buffer);
    event_t ev1(ev.header);
    ev1.append_local_station(ls);
    ev1.write_to(ofs);
    
}