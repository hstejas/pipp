#ifndef MY_RASPBERRYPI_GPIO_H
#define MY_RASPBERRYPI_GPIO_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>
#include <chrono>
#include <bitset>
#include <memory>

namespace pi
{
typedef std::chrono::milliseconds ms;
typedef std::chrono::microseconds us;

enum class Direction
{
    IN = 0, OUT
};

enum class GpioEvent
{
    HIGH2LOW=0,
    LOW2HIGH
};

const uint32_t BCM2708_PERI_BASE_ADDR = 0x20000000;
const uint32_t GPIO_BASE_ADDR = BCM2708_PERI_BASE_ADDR + 0x200000;
const uint32_t BLOCK_SIZE = 4 * 1024;

typedef void(*foo)();

struct GpioHelper
{
    GpioHelper();
    ~GpioHelper();
    int _gpiomemFd;
    volatile uint32_t* _gpioAddr;
};

class Gpio
{
private:
    static std::shared_ptr<GpioHelper> _gpioHelper;
    uint8_t _pin;
public:
    Gpio(uint8_t pin);
    Gpio(uint8_t pin, const Direction& dir);

    void setDirection(const Direction& dir);

    std::bitset<1> get() const;
    void set(const std::bitset<1>& in);
    void toggle();

    Gpio& operator=(const std::bitset<1>& in);
    Gpio& operator=(Gpio& in);

    bool operator==(const std::bitset<1>& in) { return get() == in; }
    bool operator==(const Gpio& in) { return get() == in.get(); }

    void high();
    void low();
    bool isHigh();
    bool isLow();

    void setCallback(GpioEvent& event, foo func);

protected:
    void init(uint8_t pin);
};

}
#endif // MY_RASPBERRYPI_GPIO_H
