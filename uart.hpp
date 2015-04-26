#ifndef MY_RASPBERRYPI_UART_H
#define MY_RASPBERRYPI_UART_H

#include <string>

namespace pi
{
	
class Uart
{
public:
    enum Error{INVALID_BAUD, CANOT_OPEN};

	Uart(unsigned int baud, std::string device);
	~Uart();
	
	void read(std::string&);
	int read();
	
	bool isOpen();

private:

    bool _isOpen;
    unsigned int _fd;
    
};

}
#endif //MY_RASPBERRYPI_UART_H