#ifndef MY_RASPBERRYPI_BITS_H
#define MY_RASPBERRYPI_BITS_H

#include <pi/gpio.hpp>
#include <vector>

namespace pi
{

namespace
{

    template<int I, int... Args>    struct element;

    template<int I>
    struct element<I>
    {
        void get(const std::bitset<I>& in){}
        void set(const std::bitset<I>& in){}
        void setDirection(const Direction& in){}
    };

    template<int I, int GPIO, int... args>
    struct element<I, GPIO, args...> : public element < I+1, args...>
    {
        Gpio<GPIO> _bit;

        Gpio<GPIO> getBit()
        {
            return _bit;
        }

        void get(std::bitset<I+sizeof...(args)+1>& in)
        {
            in[I] = _bit.get().test(0);
            element<I+1, args...>::get(in);
        }

        void set(const std::bitset<I+sizeof...(args)+1>& in)
        {
            _bit = in[I];
            element<I+1, args...>::set(in);
        }

        void setDirection(const Direction& dir)
        {
            _bit.setDirection(dir);
            element<I+1, args...>::setDirection(dir);
        }
    };
}


template<int... args>
struct Bits : public element<0, args...>
{

public:

    size_t length(){return sizeof...(args);}

    template<int INDEX>
    auto getBit() ->  decltype(element<INDEX,args...>::_bit)
    {
        return element<INDEX, args...>::_bit;
    }

    template<int INDEX>
    auto get() -> decltype(this->element<INDEX,args...>::_bit.get())
    {
        return this->element<INDEX, args...>::_bit.get();
    }

    std::bitset<sizeof...(args)> get()
    {
        std::bitset<sizeof...(args)> ret;
        element<0, args...>::get(ret);
        return ret;
    }

    template<int INDEX>
    void set(const std::bitset<1>& in)
    {
        element<INDEX, args...>::set(in);
    }

    void set(const std::bitset<sizeof...(args)>& in)
    {
        element<0, args...>::set(in);
    }

    std::bitset<sizeof...(args)> operator=(const std::bitset<sizeof...(args)>& in)
    {
        set(in);
        return in;
    }

    void setDirection(const Direction dir)
    {
        element<0, args...>::setDirection(dir);
    }
};

}
#endif //MY_RASPBERRYPI_BITS_H
