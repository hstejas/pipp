#ifndef MY_RASPBERRYPI_UART_H
#define MY_RASPBERRYPI_UART_H

#include <string>
#include <map>

namespace pi
{
typedef std::map<unsigned int, unsigned int> baudMap;

class Uart
{
public:
    enum Error{INVALID_BAUD, CANNOT_OPEN};
    Uart();
    Uart(unsigned int baud, std::string device = "/dev/ttyAMA0");
    ~Uart();
    
    void open(unsigned int baud, std::string device = "/dev/ttyAMA0");
    void read(std::string& str, const uint8_t& till='\0');
    bool read(uint8_t& ch);
    void write(const uint8_t& ch);
    void write(const std::string& str);

    bool isOpen();

private:

    bool _isOpen;
    unsigned int _fd;
    static const baudMap _supportedBaud;
};

}
#endif //MY_RASPBERRYPI_UART_H
