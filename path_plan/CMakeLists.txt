project(examples)
cmake_minimum_required(VERSION 2.8)

option(USE_OPENCV "option for OpenCV" OFF)

if (USE_OPENCV)
find_package(OpenCV REQUIRED)
add_definitions(-DUSE_OPENCV)
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(path_plan path_plan.cpp)
if (USE_OPENCV)
target_link_libraries(path_plan ${OpenCV_LIBS})
endif ()