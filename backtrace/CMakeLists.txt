project(backtrace)
cmake_minimum_required(VERSION 2.8)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O2")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(lib_func STATIC lib_func.c)

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../build/backtrace")

add_executable(backtrace add.cpp backtrace.cpp thread.cpp)
target_link_libraries(backtrace pthread lib_func)
