#ifndef MY_RASPBERRYPI_GPIO_H
#define MY_RASPBERRYPI_GPIO_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <exception>
#include <bitset>
#include <chrono>
#include <bitset>

#ifndef NDEBUG
#warning "OUTPUT Defined to stdout"
#define OUTPUT(x) {std::cout << "OUT@"<< __LINE__<< " F: " << __PRETTY_FUNCTION__ << "  "<<x << std::endl;}
#else
#define OUTPUT(x)
#endif



template<typename T>
inline std::string toString(const T& in)
{
    std::stringstream ss;
    ss << in;
    return ss.str();
}

namespace pi
{
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::microseconds us;
class Inception : public std::runtime_error
{
public:
    inline Inception(const std::string& in="",
              const std::string& line = toString(__LINE__) ,
              const std::string& func = toString(__PRETTY_FUNCTION__) ):std::runtime_error(in + " (" + line + ") " + func)
            {
            }
    virtual ~Inception() noexcept {}
};

enum class Direction
{
    IN = 0, OUT
};

namespace
{
const std::string PATH ="/sys/class/gpio";
}


template <int PORT>
class Gpio
{
    static Direction mDir;
    static std::bitset<1> mVal;
    static bool mUsed;
    const static std::string mDirFile;
    const static std::string mValFile;
public:
    Gpio();
    ~Gpio();
    void setDirection(const Direction& dir);
    std::bitset<1> get();
    void set(const std::bitset<1>& in);

    Gpio<PORT>& operator=(const std::bitset<1>& in);

   // template<int PORTRHS>
   // friend std::bitset<1>& operator=(std::bitset<1>& lhs, Gpio<PORTRHS>& rhs);

    template<int PORTIN>
    Gpio<PORT>& operator=(Gpio<PORTIN>& in);

    void high(){set(1);};
    void low(){set(0);};
    void isHigh(){return get() == 1;}
    void isLow(){return get() == 0;}
    void toggle(std::chrono::milliseconds& in, double dutycycle=0.5);

private:
    void init();
};

template<int PORT> Direction Gpio<PORT>::mDir;
template<int PORT> bool Gpio<PORT>::mUsed=false;
template<int PORT> std::bitset<1> Gpio<PORT>::mVal;
template<int PORT> const std::string Gpio<PORT>::mDirFile = PATH+"/gpio"+toString(PORT)+"/direction";
template<int PORT> const std::string Gpio<PORT>::mValFile = PATH+"/gpio"+toString(PORT)+"/value";


template<int PORT>
Gpio<PORT>::Gpio()
{
    if(mUsed)
        throw Inception("Port Already In Use");
    mUsed=true;
    init();
}

template<int PORT>
Gpio<PORT>::~Gpio()
{
    std::ofstream unexportFile;
    unexportFile.open((PATH+"/unexport").c_str());
    if(unexportFile)
    {
        unexportFile << PORT;
        unexportFile.close();
    }
    else
    {

    }
    mUsed=false;
}

template<int PORT>
void Gpio<PORT>::init()
{
    std::ofstream exportFile;
    exportFile.open((PATH+"/export").c_str());

    if(!exportFile)
        throw Inception("Cannot open Export file!!\n");
    exportFile << PORT;
    exportFile.close();
}

template<int PORT>
void Gpio<PORT>::setDirection(const Direction& dir)
{
    std::ofstream dirFile;
    dirFile.open(mDirFile.c_str());
    OUTPUT(mDirFile);

    if(dirFile)
    {
        std::string dirStr = dir == Direction::IN? "in" : "out";
        dirFile << dirStr;
        dirFile.close();
        mDir = dir == Direction::IN? Direction::IN : Direction::OUT;
    }
    else
    {
        throw Inception("Cannot open Direction file!!\n");
    }
}

template<int PORT>
void Gpio<PORT>::set(const std::bitset<1>& val)
{

    if(mDir != Direction::OUT)
        throw Inception("Cannot Set GPIO when Direction is not OUT");

    std::ofstream valFile;
    valFile.open(mDirFile.c_str());

    if(valFile)
    {
        mVal=val;
        valFile << val;
        valFile.close();
    }
    else
    {
        throw Inception("Cannot open Value file!!\n");
    }
}

template<int PORT>
std::bitset<1> Gpio<PORT>::get()
{
    if(mDir == Direction::IN)
    {
        std::ifstream valFile;
        valFile.open(mValFile.c_str());

        if(valFile)
        {
            valFile >> mVal;
            valFile.close();
        }
        else
        {
            throw Inception("Cannot open Value file!!\n");
        }
    }
    return mVal;
}

template<int PORT>
Gpio<PORT>& Gpio<PORT>::operator=(const std::bitset<1>& in)
{
    set(in);
    return *this;
}

template<int PORT>
template<int PORTIN>
Gpio<PORT>& Gpio<PORT>::operator=(Gpio<PORTIN>& in)
{
    set(in.get());
    return *this;
}

/*template<int PORTRHS>
std::bitset<1>& operator=(std::bitset<1>& lhs, Gpio<PORTRHS>& rhs)
{
	lhs = rhs.get();
	return lhs;
}*/

}
#endif // MY_RASPBERRYPI_GPIO_H
