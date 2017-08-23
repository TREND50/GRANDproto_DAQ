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
    event_header_t eh;
    //ifs.read((char*)&eh, sizeof(eh));
    ev.read_from(ifs);
    std::cout<<"==========="<<std::endl;
    std::cout<<ev<<std::endl;
}