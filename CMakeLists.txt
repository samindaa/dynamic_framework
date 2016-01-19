cmake_minimum_required(VERSION 3.4.1)
project(fwx)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

list( APPEND CMAKE_CXX_FLAGS "-Wno-deprecated -ggdb -O3 -std=c++0x -U_FORTIFY_SOURCE -D_GLIBCXX_USE_NANOSLEEP")

file(GLOB FWX_SOURCES1 "*.cpp")
file(GLOB FWX_SOURCES2 "examples/mod/*.cpp")
set( FWX_SOURCES "")
list(APPEND FWX_SOURCES ${FWX_SOURCES1})
list(APPEND FWX_SOURCES ${FWX_SOURCES2})


set (FWX_INCLUDE_DIRS ".")
list (APPEND FWX_INCLUDE_DIRS "examples")
list(REMOVE_DUPLICATES FWX_INCLUDE_DIRS)
include_directories(${FWX_INCLUDE_DIRS})

find_package(Threads)
add_executable(fwx ${FWX_SOURCES})