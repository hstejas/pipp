#include <pi/gpio.hpp>
#include <chrono>
#include <thread>

namespace pi
{
namespace suart
{
	template<int BAUD, int PIN>
	uint8_t get()
	{
		Gpio<PIN> RX;
		uint8_t ret(0);
		RX.setDirection(Direction::IN);

		while(RX.get() == 1);

        std::this_thread::sleep_for(pi::us(long((1000000*1.5)/(BAUD))));
        for( int i=0; i< 8 ;i++)
        {
            if(RX.get() == 1)
            {
                ret = (ret >> 1) | 0x80;
            }
            else
            {
                ret = ret >> 1;
            }
            std::this_thread::sleep_for(pi::us(long((1000000*1)/(BAUD))));
        }
        return ret;
	}
    
    template<int BAUD, int PIN>
	void put(uint8_t out)
	{
	}
}
}
