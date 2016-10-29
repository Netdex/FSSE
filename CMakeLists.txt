cmake_minimum_required(VERSION 3.6)
project(FSSE)
set(TEST TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static -static-libgcc -static-libstdc++")
if(TEST)
    add_definitions(-DTEST)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif()

set(SOURCE_FILES main.cpp fsse_util.cpp fsse_util.h)
add_executable(FSSE ${SOURCE_FILES})