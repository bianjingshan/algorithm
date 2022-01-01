project(examples)
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GLIB_INCLUDE_DIRS})

add_executable(glib_option option.cpp)

target_link_libraries(glib_option ${GLIB_LIBRARIES})
