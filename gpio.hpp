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



template<typename T>
std::string toString(const T& in)
{
    std::stringstream ss;
    ss << in;
    return ss.str();
}

namespace pi
{

class Inception : public std::runtime_error
{
public:
    Inception(const std::string& in="",
              const std::string& line = toString(__LINE__) ,
              const std::string& func=__PRETTY_FUNCTION__ ):std::runtime_error(in + " (" + line + ") " + func) { }
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
class Gpio //: public GpioBase
{
    const static int mPinNo = PORT;
    static Direction mDir;
    static std::bitset<1> mVal;
public:
    Gpio();
    ~Gpio();
    void setDirection(const Direction& dir);
    std::bitset<1> get();
    void set(const std::bitset<1>& in);

    void high(){set(1);};
	void low(){set(0);};
    void isHigh(){return get() == 1;}  
    void isLow(){return get() == 0;}
    void toggle(std::chrono::milliseconds& in, double dutycycle=0.5);

private:
    void init();
};

template<int PORT>
Gpio<PORT>::Gpio()
{
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
    dirFile.open((PATH+"/gpio"+toString(PORT)+"/direction").c_str());

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
    valFile.open((PATH+"/gpio"+toString(PORT)+"/value").c_str());

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
		valFile.open((PATH+"/gpio"+toString(PORT)+"/value").c_str());

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

}
#endif // MY_RASPBERRYPI_GPIO_H
