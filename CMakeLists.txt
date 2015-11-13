#Specipecify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(pi)

#Sends the -std=c++11 flag to the gcc compiler
add_definitions(-std=c++11)
include_directories(.)
add_library( pi STATIC utils.hpp gpio.cpp gpio.hpp bits.hpp pwm.hpp lcd.hpp uart.cpp uart.hpp)