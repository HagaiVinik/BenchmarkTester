cmake_minimum_required(VERSION 3.17)
project(BenchmarkTester)

set(CMAKE_CXX_STANDARD 14)


include_directories(BTpcppDPDK/HeaderFiles)
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

file(GLOB_RECURSE BTPCPP_DPDK_HEADERS BTpcppDPDK/HeaderFiles/*.hpp)
file(GLOB_RECURSE BTPCPP_DPDK_SOURCES BTpcppDPDK/SourceFiles/*.cpp)

add_executable(BenchmarkTester main.cpp
        ${BTPCPP_DPDK_HEADERS}
        ${BTPCPP_DPDK_SOURCES}
        ${BTSENDER_HEADERS}
        ${BTSENDER_SOURCES}
        ${BTRECEIVER_DPDK_HEADERS}
        ${BTRECEIVER_DPDK_SOURCES})


include_directories(/usr/local/include/pcapplusplus)
include_directories(/usr/local/include/dpdk)
include_directories(/home/hagai/Downloads/dpdk-stable-18.11.10/build/include)
link_directories(/home/hagai/Downloads/dpdk-stable-18.11.10/build/lib)

set(DPDK_LIBS "-Wl,--whole-archive -ldpdk -Wl,--no-whole-archive")

target_link_libraries(${PROJECT_NAME} ${DPDK_LIBS})
target_link_libraries(${PROJECT_NAME} Pcap++ Packet++ Common++ pcap)
target_link_libraries(${PROJECT_NAME} pthread dl rt m numa)




