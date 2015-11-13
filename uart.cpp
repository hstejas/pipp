#include <utils.hpp>
#include <uart.hpp>
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


pi::baudMap baudInit()
{
        pi::baudMap m;
        m[50] = B50;
        m[75] = B75;
        m[110] = B110;
        m[134] = B134;
        m[150] = B150;
        m[200] = B200;
        m[300] = B300;
        m[600] = B600;
        m[1200] = B1200;
        m[1800] = B1800;
        m[2400] = B2400;
        m[4800] = B4800;
        m[9600] = B9600;
        m[19200] = B19200;
        m[38400] = B38400;
        m[57600] = B57600;
        m[115200] = B115200;
        m[230400] = B230400;
        return m;
}

const pi::baudMap pi::Uart::_supportedBaud = baudInit();

pi::Uart::Uart() : _isOpen(false)
{
}

pi::Uart::Uart(unsigned int baud, std::string device /*= "/dev/ttyAMA0"*/) : _isOpen(false)
{
    this->open(baud, device);
}

void pi::Uart::open(unsigned int baud, std::string device /*= "/dev/ttyAMA0"*/)
{
    termios options ;
    speed_t termBaud ;
    int     status;
    DEBUG("BAUD: " << baud << " termBaud: " << termBaud << "  device: " << device);
    baudMap::const_iterator it = _supportedBaud.find(baud);

    if(it == _supportedBaud.end())
        return;

    termBaud = it->second;

    if ((_fd = ::open (device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
    return;

    fcntl (_fd, F_SETFL, O_RDWR) ;

    tcgetattr (_fd, &options) ;

    cfmakeraw   (&options) ;
    cfsetispeed (&options, termBaud) ;
    cfsetospeed (&options, termBaud) ;

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
    DEBUG("FD : " << _fd);

    _isOpen=true;
}

pi::Uart::~Uart()
{
    ::close (_fd) ;
}

bool pi::Uart::isOpen()
{
    return _isOpen;
}

void pi::Uart::read(std::string& str, const uint8_t& till/*='\0'*/)
{
    uint8_t ch;
    while (this->read(ch))
    {
        if (till == ch)
            break;
        str += ch;
    }
}

bool pi::Uart::read(uint8_t& ch)
{
    uint8_t x ;
    if (::read (_fd, &x, 1) != 1)
        return false ;
    DEBUG("READ: " << x);
    ch = x;
    return true;
}

void pi::Uart::write(const uint8_t& ch)
{
    DEBUG("Write: " << ch);
    ::write(_fd, &ch, 1);
}

void pi::Uart::write(const std::string& str)
{
    DEBUG("Write: " << str.c_str() << " Len: " << str.length());
    if (str.length() > 0)
    {
        ::write(_fd, str.c_str(), str.length()+1);
    }
}
