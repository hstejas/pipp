#include <pi/bits.hpp>
#include <thread>

int main()
{
    pi::Bits<2> bits({ 20, 21 }, pi::Direction::OUT);
    unsigned int i = 0;
    while (1)
    {
        bits = i++;
        if (i > 3) i = 0;
        std::this_thread::sleep_for(pi::ms(1000));
    }
}
