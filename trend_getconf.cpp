#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

char port[] = "1234";
char IPadr[] = "192.168.1.18";

int main (int argc, char *argv[])
{
    std::cout << "Sending TRENDSLCReq message to " << IPadr << ":" << port << std::endl;

    boost::system::error_code error;
    boost::asio::io_service io_service;

    udp::socket socket (io_service);
    // Edit the message you want to send in following data[]
    // either you can use unsigned char type or unsigned int


    // TRENDSLC message
    uint32_t data[] = { 0xaaaaaaaa, 0x00005200, 0xaaaaaaaa };

    socket.open (boost::asio::ip::udp::v4 (), error);

    if (!error)
        {
            socket.set_option (boost::asio::ip::udp::socket::reuse_address (true));
            socket.set_option (boost::asio::socket_base::broadcast (true));
            boost::asio::io_service io_service;
            udp::resolver resolver (io_service);
            udp::endpoint endpoint = *resolver.resolve ({ udp::v4 (), IPadr, port });


            socket.send_to (boost::asio::buffer (data, sizeof (data)), endpoint);
            socket.close (error);
            std::cerr << error << std::endl;
        }
    return 0;
}
