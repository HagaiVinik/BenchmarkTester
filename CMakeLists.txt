cmake_minimum_required(VERSION 3.17)
project(BenchmarkTester)

set(CMAKE_CXX_STANDARD 14)


include_directories(HeaderFiles)
include_directories(BTReceiver/HeaderFiles)
include_directories(BTSender/HeaderFiles)

# Will add this later:
#       BTReceiver/temp/BTServer.h
#        BTReceiver/temp/BTServer.cpp
#        BTReceiver/temp/PacketStats.h
#        BTReceiver/temp/PacketStats.cpp


file(GLOB_RECURSE BTRECEIVER_HEADERS BTReceiver/HeaderFiles/*.hpp)
file(GLOB_RECURSE BTRECEIVER_SOURCES BTReceiver/SourceFiles/*.cpp)
file(GLOB_RECURSE BTSENDER_HEADERS BTSender/HeaderFiles/*.hpp)
file(GLOB_RECURSE BTSENDER_SOURCES BTSender/SourceFiles/*.cpp)

add_executable(BenchmarkTester main.cpp
        ${BTRECEIVER_HEADERS}
        ${BTRECEIVER_SOURCES}
        ${BTSENDER_HEADERS}
        ${BTSENDER_SOURCES})


include_directories(/usr/local/include/pcapplusplus)

target_link_libraries(${PROJECT_NAME} Pcap++ Packet++ Common++ pcap)
target_link_libraries(${PROJECT_NAME} pthread )




