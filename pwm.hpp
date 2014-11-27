#ifndef MY_RASPBERRYPI_PWM_H
#define MY_RASPBERRYPI_PWM_H

#include <pi/gpio.hpp>
#include <thread>
#include <chrono>

namespace pi
{

template<int PORT>
class PWM
{
public:
    PWM();

    void start();
    void setPeriod(pi::us period){ mPeriod = period; }
    void setDutyCycle(float dc){ mDutyCycle = dc; }
    void stop();

    void toggle();
        
protected:
    bool mStart;
    float mDutyCycle;
    pi::us mPeriod;
    Gpio<PORT> mPwmPin;
    std::thread mToggleThread;
};

template<int PORT>
void PWM<PORT>::toggle()
{
    while (mStart)
    {
        mPwmPin = 0;
        std::this_thread::sleep_for(pi::us(static_cast<unsigned int>(mPeriod.count() * mDutyCycle)));
        mPwmPin = 1;
        std::this_thread::sleep_for(pi::us(static_cast<unsigned int>(mPeriod.count() * (1 - mDutyCycle))));
    }
}

template<int PORT>
PWM<PORT>::PWM()
{
    mStart = false;
    mPeriod = pi::ms(1000);
    mDutyCycle = 0.5;
}

template<int PORT>
void PWM<PORT>::start()
{
    if (!mStart)
    {
        mStart = true;
        mPwmPin.setDirection(Direction::OUT);
        mToggleThread = std::thread(&PWM<PORT>::toggle, this);;
    }
}

template<int PORT>
void PWM<PORT>::stop()
{
    if (mStart)
    {
        mStart = false;
        mToggleThread.join();
    }
}

}


#endif // MY_RASPBERRYPI_PWM_H