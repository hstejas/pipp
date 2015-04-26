#include <pi/uart.hpp>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstdarg>
#include <cstring>


pi::Uart::Uart(unsigned int baud, std::string device) : _isOpen(false)
{
    termios options ;
    speed_t myBaud ;
    int     status;

    switch (baud)
    {
    case     50:	myBaud =     B50 ; break ;
    case     75:	myBaud =     B75 ; break ;
    case    110:	myBaud =    B110 ; break ;
    case    134:	myBaud =    B134 ; break ;
    case    150:	myBaud =    B150 ; break ;
    case    200:	myBaud =    B200 ; break ;
    case    300:	myBaud =    B300 ; break ;
    case    600:	myBaud =    B600 ; break ;
    case   1200:	myBaud =   B1200 ; break ;
    case   1800:	myBaud =   B1800 ; break ;
    case   2400:	myBaud =   B2400 ; break ;
    case   4800:	myBaud =   B4800 ; break ;
    case   9600:	myBaud =   B9600 ; break ;
    case  19200:	myBaud =  B19200 ; break ;
    case  38400:	myBaud =  B38400 ; break ;
    case  57600:	myBaud =  B57600 ; break ;
    case 115200:	myBaud = B115200 ; break ;
    case 230400:	myBaud = B230400 ; break ;

    default:
    return;
    }

    if ((_fd = open (device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
    return;

    fcntl (_fd, F_SETFL, O_RDWR) ;

    tcgetattr (_fd, &options) ;

    cfmakeraw   (&options) ;
    cfsetispeed (&options, myBaud) ;
    cfsetospeed (&options, myBaud) ;

    options.c_cflag |= (CLOCAL | CREAD) ;
    options.c_cflag &= ~PARENB ;
    options.c_cflag &= ~CSTOPB ;
    options.c_cflag &= ~CSIZE ;
    options.c_cflag |= CS8 ;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
    options.c_oflag &= ~OPOST ;

    options.c_cc [VMIN]  =   0 ;
    options.c_cc [VTIME] = 100 ;

    tcsetattr (_fd, TCSANOW | TCSAFLUSH, &options) ;

    ioctl (_fd, TIOCMGET, &status);

    status |= TIOCM_DTR ;
    status |= TIOCM_RTS ;

    ioctl (_fd, TIOCMSET, &status);

    usleep (10000) ;

    _isOpen=true;
}
pi::Uart::~Uart()
{
	close (_fd) ;
}

bool pi::Uart::isOpen()
{
    return _isOpen;
}
	
void pi::Uart::read(std::string& )
{
	
}
int pi::Uart::read()
{
    uint8_t x ;

    if (::read (_fd, &x, 1) != 1)
        return -1 ;

    return ((int)x) & 0xFF ;
}