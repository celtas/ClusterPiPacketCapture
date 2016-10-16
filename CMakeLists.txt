cmake_minimum_required(VERSION 3.5)
project(CIT_MonoMake)

#For mysql connector include..
INCLUDE_DIRECTORIES(/usr/local/Cellar/mysql-connector-c++/mysqlconnector/include/)
#For Boost..
find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpcap")

set(SOURCE_FILES main/Trigger.cpp packet/PacketCapture.cpp packet/PacketCapture.h mysql/DatabaseManager.cpp mysql/DatabaseManager.h packet/PortAPI.cpp packet/PortAPI.h)
add_executable(CIT_MonoMake ${SOURCE_FILES})

target_link_libraries (CIT_MonoMake /usr/local/Cellar/mysql-connector-c++/mysqlconnector/lib/libmysqlcppconn-static.a)
target_link_libraries(CIT_MonoMake ${Boost_LIBRARIES})