#include <pi/bits.hpp>
#include <chrono>
#include <thread>


namespace pi
{
    typedef std::chrono::milliseconds ms;
    ms LCD_DELAY(1);
    unsigned int LINE1_RAM(0x80);
    unsigned int LINE2_RAM(0xc0);
    //unsigned int LINE3_RAM(0x80);
    //unsigned int LINE4_RAM(0x80);

template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int CharCount>
class Lcd
{
    Bits<P1,P2,P3,P4> _data;
    Gpio<EN> _en;
    Gpio<RS> _rs;


    inline void delay(const std::chrono::milliseconds& d = LCD_DELAY);

protected:
    inline void latch();
    inline void writeNibble(const std::bitset<4>& in);

public:
    Lcd();
    ~Lcd(){};
    void init();
    void write(unsigned int line, std::string str);
    inline void command(const std::bitset<8>& cmd);
    inline void data(const std::bitset<8>& cmd);
};
template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int Length>
Lcd<P1,P2,P3,P4,EN,RS,Lines,Length>::Lcd()
{
    _data.setDirection(Direction::OUT);
    _en.setDirection(Direction::OUT);
    _rs.setDirection(Direction::OUT);
}


template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int Length>
void Lcd<P1,P2,P3,P4,EN,RS,Lines,Length>::init()
{
    _en = _rs = 0;
    _data = 0;
    delay(ms(200));

    // Configure for 4 bit mode
    writeNibble(0x3);
    writeNibble(0x3);
    writeNibble(0x3);
    writeNibble(0x2);
    command(0x28);
    command(0x0c);
    command(0x06);
    command(0x1);
}

template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int Length>
inline void Lcd<P1,P2,P3,P4,EN,RS,Lines,Length>::latch()
{
    _en=1;delay();
    _en=0;delay();
}

template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int Length>
inline void Lcd<P1,P2,P3,P4,EN,RS,Lines,Length>::writeNibble(const std::bitset<4>& in)
{
    _data=in;
    latch();
}


template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int Length>
inline void Lcd<P1,P2,P3,P4,EN,RS,Lines,Length>::delay(const std::chrono::milliseconds& d /*= LCD_DELAY*/)
{
    std::this_thread::sleep_for(d);
}

template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int Length>
void Lcd<P1,P2,P3,P4,EN,RS,Lines,Length>::command(const std::bitset<8>& cmd)
{
    _en = _rs = 0;
    writeNibble(std::bitset<4>((cmd.to_ulong() & 0xf0) >> 4));
    writeNibble(std::bitset<4>(cmd.to_ulong() & 0x0f));
}

template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int Length>
void Lcd<P1,P2,P3,P4,EN,RS,Lines,Length>::data(const std::bitset<8>& dat)
{
    _en = 0; _rs = 1;
    writeNibble(std::bitset<4>((dat.to_ulong() & 0xf0) >> 4));
    writeNibble(std::bitset<4>(dat.to_ulong() & 0x0f));
    _rs = 0;
}
template<int P1, int P2, int P3, int P4, int EN, int RS, int Lines, int Length>
void Lcd<P1,P2,P3,P4,EN,RS,Lines,Length>::write(unsigned int line, std::string str)
{
    if(line > Lines || line == 0)
        throw Inception(std::string("Invalid Line number: ")+toString(line)+" \n");
    switch(line)
    {
    case 1:
        line = 0x80;break;
    case 2:
        line = 0xc0;break;
    }
    command(line);
    OUTPUT("\nLINE: "<< std::hex << line)
    for(int i=0;i<str.length();i++)
        data(str[i]);
}



}

