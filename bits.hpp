#ifndef MY_RASPBERRYPI_BITS_H
#define MY_RASPBERRYPI_BITS_H

#include <vector>
#include <array>
#include <gpio.hpp>

namespace pi
{
template <uint8_t BITS_SIZE>
class Bits
{
public:
    Bits(std::array<uint8_t, BITS_SIZE>  gpioList);
    Bits(const Bits<BITS_SIZE>&) = delete;
    Bits(std::array<uint8_t, BITS_SIZE>  gpioList, Direction dir);

    void set(const std::bitset<BITS_SIZE>& in);
    std::bitset<BITS_SIZE> get() const;

    void setDirection(Direction dir);

    Bits& operator=(const std::bitset<BITS_SIZE>& in) { set(in); return *this; }
    bool operator==(const std::bitset<BITS_SIZE>& in);
        
    //TODO
    //std::bitset<1> operator[] (unsigned int index) const; //Rvalue
    //void operator[](std::bitset<1> val); // Lvalue

protected:
    std::vector<Gpio> _gpios;
};

template<uint8_t BITS_SIZE>
Bits<BITS_SIZE>::Bits(std::array<uint8_t, BITS_SIZE>  gpioList)
{
    for (unsigned int i = 0; i < gpioList.size(); i++)
        _gpios.push_back(Gpio(gpioList[i]));
}

template<uint8_t BITS_SIZE>
Bits<BITS_SIZE>::Bits(std::array<uint8_t, BITS_SIZE>  gpioList, Direction dir)
{
    for (unsigned int i = 0; i < gpioList.size(); i++)
        _gpios.push_back(std::move(Gpio(gpioList[i], dir)));
}

template<uint8_t BITS_SIZE>
void Bits<BITS_SIZE>::set(const std::bitset<BITS_SIZE>& in)
{
    for (unsigned int i = 0; i < _gpios.size(); i++)
    {
        _gpios[i] = in[i];
    }
}

template<uint8_t BITS_SIZE>
std::bitset<BITS_SIZE> Bits<BITS_SIZE>::get() const
{
    std::bitset<BITS_SIZE> out;
    for (unsigned int i = 0; i < _gpios.size(); i++)
    {
        out[i] = _gpios[i];
    }
    return out;
}

template<uint8_t BITS_SIZE>
void Bits<BITS_SIZE>::setDirection(Direction dir)
{
    for (unsigned int i = 0; i < _gpios.size(); i++)
        _gpios[i].setDirection(dir);
}

template<uint8_t BITS_SIZE>
bool Bits<BITS_SIZE>::operator==(const std::bitset<BITS_SIZE>& in)
{
    bool ret = true;
    for (unsigned int i = 0; i < _gpios.size(); i++)
    {
        if (_gpios[i] != in[i])
        {
            ret = false;
            break;
        }
    }
    return ret;
}
}
#endif //MY_RASPBERRYPI_BITS_H
