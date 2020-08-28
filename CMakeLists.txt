cmake_minimum_required(VERSION 2.8)
project(algorithm)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(path_plan)
add_subdirectory(lock)
add_subdirectory(map)
add_subdirectory(interview)
