cmake_minimum_required(VERSION 3.17)
project(BenchmarkTester)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)


include_directories(HeaderFiles)

add_executable(BenchmarkTester main.cpp
        HeaderFiles/BTServer.h
        SourceFiles/BTServer.cpp
        HeaderFiles/PacketStats.h
        SourceFiles/PacketStats.cpp

        BTReceiver/HeaderFiles/BTReceiver.h
        BTReceiver/SourceFiles/BTReceiver.cpp
        BTReceiver/HeaderFiles/BTReceiverTCP.h
        BTReceiver/SourceFiles/BTReceiverTCP.cpp
        BTReceiver/HeaderFiles/BTReceiverUDP.h
        BTReceiver/SourceFiles/BTReceiverUDP.cpp

        BTSender/HeaderFiles/BTSender.h
        BTSender/SourceFiles/BTSender.cpp
        BTSender/HeaderFiles/BTSenderTCP.h
        BTSender/SourceFiles/BTSenderTCP.cpp
        BTSender/SourceFiles/BTSenderUDP.cpp
        BTSender/HeaderFiles/BTSenderUDP.h
        BTReceiver/BTReceiverDPDK/SourceFiles/BTReceiverDPDK.cpp
        BTReceiver/BTReceiverDPDK/HeaderFiles/BTReceiverDPDK.h
        BTReceiver/BTReceiverDPDK/SourceFiles/L2FwdWorkerThread.cpp
        BTReceiver/BTReceiverDPDK/HeaderFiles/L2FwdWorkerThread.h )


include_directories(/usr/local/include/pcapplusplus)
include_directories(/home/hagai/Downloads/dpdk-stable-18.11.10/build/include)
link_directories(/home/hagai/Downloads/dpdk-stable-18.11.10/build/lib)

set(DPDK_LIBS "-Wl,--whole-archive -ldpdk -Wl,--no-whole-archive")

target_link_libraries(${PROJECT_NAME} ${DPDK_LIBS})
target_link_libraries(${PROJECT_NAME} Pcap++ Packet++ Common++ pcap)
target_link_libraries(${PROJECT_NAME} pthread dl rt m numa)




