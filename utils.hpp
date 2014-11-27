#ifndef MY_RASPBERRYPI_UTILS_H
#define MY_RASPBERRYPI_UTILS_H

#include <stdexcept>
#include <exception>
#include <iostream>
#include <sstream>

namespace pi
{
	#define	DEBUG(x) do{ std::cout << x << std::endl; }while(false);


template<typename T>
inline std::string toString(const T& in)
{
    std::stringstream ss;
    ss << in;
    return ss.str();
}

template<typename T>
inline T fromString(const std::string& in)
{
    std::stringstream ss(in);
    T out;
    ss >> out;
    return out;
}

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
}
#endif // MY_RASPBERRYPI_UTILS_H
