#include <pi/gpio.hpp>
#include <thread>

int main()
{
    pi::Gpio p20(20, pi::Direction::OUT);
    pi::Gpio p21(21, pi::Direction::IN);
    while (1)
    {
        if (p21 == 0)
        {
            return 0;
        }
        p20.toggle();
        std::this_thread::sleep_for(pi::ms(1000));
    }
}
