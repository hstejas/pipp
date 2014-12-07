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
		void get(std::bitset<I>& in){}    
    };

    template<int I, int GPIO, int... args>
    struct element<I, GPIO, args...> : public element < I+1, args...>
    {
        Gpio<GPIO> _bit;
        Gpio<GPIO> getBit(){return _bit;}
		void get(std::bitset<I+sizeof...(args)>& in){in[I] = _bit.get(); element<I+1, args...>::get(in);}
    };
}


template<int... args>
struct bits : public element<0, args...>
{

public:

    size_t length(){return sizeof...(args);}

    template<int INDEX>
    auto getBit() -> decltype(element<INDEX,args...>::get())
    {
        return element<INDEX,args...>::_bit;
    }

    template<int INDEX>
    auto get() -> decltype(element<INDEX,args...>::_bit.get())
    {
        return element<INDEX,args...>::_bit.get();
    }
	
	std::bitset<sizeof...(args)> get()
	{
		std::bitset<sizeof...(args)> ret;
		element<0,args...>::get(ret);
		return ret;
	}
};

}
#endif //MY_RASPBERRYPI_BITS_H
