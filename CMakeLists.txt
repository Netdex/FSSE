cmake_minimum_required(VERSION 3.6)
project(FSSE)
set(TEST FALSE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static -static-libgcc -static-libstdc++")
if(TEST)
    add_definitions(-DTEST)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif()

configure_file(${CMAKE_SOURCE_DIR}/95.ico ${CMAKE_CURRENT_BINARY_DIR}/95.ico COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/rcedit.exe ${CMAKE_CURRENT_BINARY_DIR}/rcedit.exe COPYONLY)

set(SOURCE_FILES main.cpp fsse_util.cpp fsse_util.h)
add_executable(FSSE ${SOURCE_FILES})

add_custom_command(TARGET FSSE
        POST_BUILD
        COMMAND rcedit FSSE.exe --set-icon 95.ico)