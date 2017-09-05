#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

char port[] = "4242"; // Slow contro read port
char IPadr[] = "192.168.1.101";

int main (int argc, char *argv[])
{
    std::cout << "Sending TRENDIntReg message to " << IPadr << ":" << port << std::endl;

    boost::system::error_code error;
    boost::asio::io_service io_service;

    udp::socket socket (io_service);
    // Edit the message you want to send in following data[]
    // either you can use unsigned char type or unsigned int

    // TRENDIntReg message ==> bind MAC & IP adresses
    uint32_t gedek[] = { 0xaaaaaaaa, 0x00005e00,
                         0x00000001, // Write to board
                         0x1238847c, // Board MAC adress (LSB)
                         0xc0a80112, // Board IP adress 192.168.1.18
                         0x186148e9, // PC1 MAC adress (LSB)
                         0x00000010, // PC1 MAC adress (MSB)
                         0xc0a80101, // PC1 IP adress 192.168.1.1
                         0x186148e9, // PC2 MAC adress (LSB)
                         0x00000010, // PC2 MAC adress (MSB)
                         0xc0a80101, // PC2 IP adress
                         0x000004d4, // Port 1 = 1235 (Data)
                         0x000004d3, // Port 2 = 1236 (SC)
                         0xaaaaaaaa };
    uint32_t gedekr[] = { 0xaaaaaaaa, 0x00005e00,
                          0x00000000, // Read from board
                          0xaaaaaaaa };

    uint32_t *data[] = { gedek, gedekr };
    uint sizes[] = { sizeof (gedek), sizeof (gedekr) };
    socket.open (boost::asio::ip::udp::v4 (), error);

    if (!error)
        {
            socket.set_option (boost::asio::ip::udp::socket::reuse_address (true));
            socket.set_option (boost::asio::socket_base::broadcast (true));
            boost::asio::io_service io_service;
            udp::resolver resolver (io_service);
            udp::endpoint endpoint = *resolver.resolve ({ udp::v4 (), IPadr, port });

            for (int i = 0; i < sizeof (sizes) / sizeof (*sizes); i++)
                {
                    std::cout << "Sending mesage " << i << std::endl;
                    socket.send_to (boost::asio::buffer (data[i], sizes[i]), endpoint);
                    usleep (1e6);
                }
            socket.close (error);
            std::cerr << error << std::endl;
        }
    return 0;
}
