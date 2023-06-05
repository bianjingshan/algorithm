project(examples)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(file_lock main.cpp file_lock.cpp)
