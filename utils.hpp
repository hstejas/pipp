#ifndef MY_RASPBERRYPI_UTILS_H
#define MY_RASPBERRYPI_UTILS_H

#include <stdexcept>
#include <exception>
#include <iostream>
#include <iomanip>
#include <sstream>


#ifdef PIDEBUG
#warning "DEBUG Defined to stdout"
#define DEBUG(x) std::cout << __PRETTY_FUNCTION__ << "[L:" << __LINE__ << "]  " << x << std::endl;
#else
#define DEBUG(x)
#endif

#ifdef PIOUTPUT
#warning "OUTPUT Defined to stdout"
#define OUTPUT(x) std::cout << "OUT@"<< __LINE__<< " F: " << __PRETTY_FUNCTION__ << "  "<<x << std::endl;
#else
#define OUTPUT(x)
#endif

namespace pi
{

template<typename T>
inline std::string toString(const T& in)
{
    std::stringstream ss;
    ss << in;
    return ss.str();
}

inline std::string toString(const float& in, const int precision)
{
    std::stringstream ss;
    ss << std::setprecision(precision) << std::fixed << in;
    return ss.str();
}

inline std::string toString(const int& in, const int width, const char fill)
{
    std::stringstream ss;
    ss << std::setw(width) << std::setfill(fill) << in;
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
