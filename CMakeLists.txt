cmake_minimum_required(VERSION 3.17)
project(BenchmarkTester)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)


include_directories(HeaderFiles)

add_executable(BenchmarkTester SourceFiles/main.cpp
        HeaderFiles/BTServer.h
        SourceFiles/BTServer.cpp
        HeaderFiles/PacketStats.h
        SourceFiles/PacketStats.cpp

        BTReceiver/HeaderFiles/BTReceiver.h
        BTReceiver/SourceFiles/BTReceiver.cpp
        BTReceiver/HeaderFiles/BTReceiverTCP.h
        BTReceiver/SourceFiles/BTReceiverTCP.cpp

        BTSender/HeaderFiles/BTSender.h
        BTSender/SourceFiles/BTSender.cpp
        BTSender/HeaderFiles/BTSenderTCP.h
        BTSender/SourceFiles/BTSenderTCP.cpp BTReceiver/SourceFiles/BTReceiverUDP.cpp BTReceiver/HeaderFiles/BTReceiverUDP.h BTSender/SourceFiles/BTSenderUDP.cpp BTSender/HeaderFiles/BTSenderUDP.h)


include_directories(/usr/local/include/pcapplusplus)

target_link_libraries(${PROJECT_NAME} Pcap++ Packet++ Common++ pcap)
target_link_libraries(${PROJECT_NAME} pthread )




