#include <iostream>
#include <string>
#define PIDEBUG
#include <pi/uart.hpp>

int main()
{
    pi::Uart uart(4800);

    if (!uart.isOpen())
        return -1;
    unsigned char ch;
    std::cout << "Type something and press enter, Ctrl+D or <EOF> to exit \n> ";
    while (std::cin >> ch)
    {
        uart.write(ch);
        unsigned char ch;
        uart.read(ch);
        std::cout << "Got: " << ch << std::endl << "> ";
    }


//while(1)
//{
//    uint8_t ch;
//    if(uart.read(ch))
//        std::cout << ch;
//}

}
