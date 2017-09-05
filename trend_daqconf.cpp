#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

char port[] = "4242"; // Slow contro read port
char IPadr[] = "192.168.1.7";

int main (int argc, char *argv[])
{
    std::cout << "Sending TRENDDAQ message to " << IPadr << ":" << port << std::endl;

    boost::system::error_code error;
    boost::asio::io_service io_service;

    udp::socket socket (io_service);
    // Edit the message you want to send in following data[]
    // either you can use unsigned char type or unsigned int

    // TRENDDAQ message
    uint32_t daq[] = { 0xaaaaaaaa, 0x00005000,
                       0x03F05ABF, // Default config is 0x03F05A93, CH3 LNA on is 0x02F05A93,
                       // Oscillator config is 03F05ABF,
                       0x00003FFF, 0xaaaaaaaa };
    // TRENDTRIG message
    uint32_t trig[] = { 0xaaaaaaaa, 0x00005100, 0x00A00001, 0x00020020,
                        0x00000000, 0x00000000, 0xaaaaaaaa };
    // TREND ADC config message
    uint32_t adc[] = { 0xaaaaaaaa, 0x00005400, 0x00002000, 0xaaaaaaaa };
    uint32_t *data[] = { daq, adc, trig };
    int sizes[] = { sizeof (daq), sizeof (adc), sizeof (trig) };
    socket.open (boost::asio::ip::udp::v4 (), error);

    if (!error)
        {
            socket.set_option (boost::asio::ip::udp::socket::reuse_address (true));
            socket.set_option (boost::asio::socket_base::broadcast (true));
            boost::asio::io_service io_service;
            udp::resolver resolver (io_service);
            udp::endpoint endpoint = *resolver.resolve ({ udp::v4 (), IPadr, port });

            for (int i = 0; i < sizeof (*sizes); i++)
                {
                    std::cout << "Sending mesage " << i << "/" << sizeof (sizes) << std::endl;
                    socket.send_to (boost::asio::buffer (data[i], sizes[i]), endpoint);
                    usleep (1e6);
                }
            socket.close (error);
            std::cerr << error << std::endl;
        }
    return 0;
}
