cmake_minimum_required(VERSION 3.6)
project(PubSub)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp PubSub.h)
add_executable(PubSub ${SOURCE_FILES})