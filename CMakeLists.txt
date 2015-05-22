cmake_minimum_required(VERSION 2.6)

project(orihime)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(amagawa)
add_subdirectory(test)
