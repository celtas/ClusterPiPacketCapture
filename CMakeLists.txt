cmake_minimum_required(VERSION 3.5)
project(CIT_MonoMake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpcap")

set(SOURCE_FILES main/Trigger.cpp packet/PacketCapture.cpp packet/PacketCapture.h mysql/DatabaseManager.cpp mysql/DatabaseManager.h)
add_executable(CIT_MonoMake ${SOURCE_FILES})