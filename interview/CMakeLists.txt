project(interview)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(mulit_thread mulit_thread.cpp)
add_executable(sync_print sync_print.cpp)
add_subdirectory(matrix_multiplication)
add_subdirectory(arithmetic_expression_parsing)
