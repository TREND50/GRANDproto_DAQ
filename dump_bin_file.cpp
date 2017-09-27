#include <fstream>
#include <iostream>
#include <cassert>
#include "data_file.hpp"

int main (int argc, char *argv[])
{
    if (argc != 2)
        {
            std::cerr << "Usage: " << argv[0] << " <bin file>" << std::endl;
            return -1;
        }
    std::ifstream ifs (argv[1]);
    event_file ef;
    ef.read_from(ifs);
    std::cout<<ef.event_list.size()<<std::endl;
    std::ofstream ofs("dump.bin");
    ef.write_to(ofs);
}
