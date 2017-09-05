#include <iostream>
#include <iomanip>
using namespace std;

int main ()
{
    cout << hex << 0x12345678 << std::endl;
    unsigned char data[] = { 0x12, 0x34, 0x56, 0x78 };
    cout << hex << *reinterpret_cast<uint32_t *> (data) << std::endl;
    uint32_t p1 = (((uint32_t)data[0]) << 24) + (((uint32_t)data[1]) << 16) +
                  (((uint32_t)data[2]) << 8) + (((uint32_t)data[3]) << 0);

    cout << hex << p1 << std::endl;
}
