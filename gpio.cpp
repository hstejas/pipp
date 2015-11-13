#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdint>
#include <unistd.h>
#include <gpio.hpp>
#include <utils.hpp>

using namespace pi;

std::shared_ptr<GpioHelper> Gpio::_gpioHelper;
const std::string MEM_PATH = "/dev/gpiomem";

GpioHelper::GpioHelper()
{
    _gpiomemFd = open(MEM_PATH.c_str(), O_RDWR | O_SYNC | O_CLOEXEC);
    if (_gpiomemFd < 0)
    {
        throw Inception(std::string("Could not open ") + MEM_PATH);
    }

    _gpioAddr = (uint32_t *)mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, _gpiomemFd, GPIO_BASE_ADDR);
    if (!_gpioAddr)
    {
        throw Inception("Could not map GPIO adderesses");
    }
}

GpioHelper::~GpioHelper()
{
    close(_gpiomemFd);
    munmap((uint32_t*)_gpioAddr, BLOCK_SIZE);
}

Gpio::Gpio(uint8_t pin)
{
    init(pin);
}

Gpio::Gpio(uint8_t pin, const Direction& dir)
{
    init(pin);
    setDirection(dir);
}

void Gpio::init(uint8_t pin)
{
    if (pin > 27)
        throw Inception("GPIO pins must be between 0 and 27");
    _pin = pin;
    if (!_gpioHelper)
        _gpioHelper = std::make_shared<GpioHelper>();
}

void Gpio::setDirection(const Direction& dir)
{
    if (dir == Direction::IN)
    {
        *(_gpioHelper->_gpioAddr + (_pin / 10)) &= ~(7 << (_pin % 10) * 3);
    }
    else
    {
        uint32_t temp = *(_gpioHelper->_gpioAddr + (_pin / 10));
        temp &= ~(7 << (_pin % 10) * 3);
        temp |= (1 << (_pin % 10) * 3);
        *(_gpioHelper->_gpioAddr + (_pin / 10)) = temp ;
    }
}

std::bitset<1> Gpio::get() const
{
    return *(_gpioHelper->_gpioAddr + 13) & (1 << _pin) ? 1 : 0;
}

void Gpio::set(const std::bitset<1>& val)
{
    if (val == 1)
        *(_gpioHelper->_gpioAddr + 7) = (1 << _pin);
    else
        *(_gpioHelper->_gpioAddr + 10) = (1 << _pin);
}

void Gpio::setCallback(GpioEvent& event, pi::foo func)
{
    if (event == GpioEvent::HIGH2LOW)
    {

    }
}

void Gpio::toggle()
{
    set(~get());
}

Gpio& Gpio::operator=(const std::bitset<1>& in)
{
    set(in);
    return *this;
}

Gpio& Gpio::operator=(Gpio& in)
{
    set(in.get());
    return *this;
}