cmake_minimum_required(VERSION 3.17)
project(BenchmarkTester)

set(CMAKE_CXX_STANDARD 14)


include_directories(BTpcppDPDK/HeaderFiles)
include_directories(BTReceiver/HeaderFiles)
include_directories(BTTransmitter/HeaderFiles)

file(GLOB_RECURSE BTRECEIVER_HEADERS BTReceiver/HeaderFiles/*.hpp)
file(GLOB_RECURSE BTRECEIVER_SOURCES BTReceiver/SourceFiles/*.cpp)
file(GLOB_RECURSE BTTRANSMITTER_HEADERS BTTransmitter/HeaderFiles/*.hpp)
file(GLOB_RECURSE BTTRANSMITTER_SOURCES BTTransmitter/SourceFiles/*.cpp)

file(GLOB_RECURSE BTPCPP_DPDK_HEADERS BTpcppDPDK/HeaderFiles/*.hpp)
file(GLOB_RECURSE BTPCPP_DPDK_SOURCES BTpcppDPDK/SourceFiles/*.cpp)

add_executable(BenchmarkTester main.cpp
        ${BTRECEIVER_HEADERS}
        ${BTRECEIVER_SOURCES}
        ${BTTRANSMITTER_HEADERS}
        ${BTTRANSMITTER_SOURCES}
        ${BTPCPP_DPDK_HEADERS}
        ${BTPCPP_DPDK_SOURCES})



### PCPP Variables ###
# Includes: #
#set(PCAPPP_INCLUDES /home/hagai/Downloads/PcapPlusPlus-20.08/Dist/header)
set(PCAPPP_INCLUDES_2 /usr/include/netinet)
# libs: #
set(PCAPPP_LIBS_DIR /home/hagai/Downloads/PcapPlusPlus-20.08/Dist)
set(PCAPPP_LIBS Pcap++ Packet++ Common++)
set(PCAPPP_LIBS pcap pthread)
#######################

### DPDK Variables ###
# Includes: #
#set(DPDK_INCLUDES /home/hagai/Downloads/dpdk-stable-18.11.10/build/include)
# Libs: #
set(DPDK_LIB /home/hagai/Downloads/dpdk-stable-18.11.10/build/lib)


include_directories(/home/hagai/Downloads/PcapPlusPlus-20.08/Dist/header)
include_directories(/usr/local/include/pcapplusplus)
include_directories(/usr/local/include/dpdk)
include_directories(/home/hagai/Downloads/dpdk-stable-18.11.10/build/include)

link_directories(/home/hagai/Downloads/dpdk-stable-18.11.10/build/lib)
link_directories(/home/hagai/Downloads/PcapPlusPlus-20.08/Dist)


set(DPDK_LIBS "-Wl,--whole-archive -ldpdk -Wl,--no-whole-archive")
set(OTHER_LIBS "-Wl,--whole-archive -lrte_pmd_bond -lrte_pmd_virtio -lrte_pmd_enic -lrte_pmd_i40e -lrte_pmd_fm10k -lrte_pmd_ixgbe -lrte_net -lrte_pmd_e1000 -lrte_pmd_ring -lrte_pmd_af_packet -lrte_kni -lrte_ethdev -lrte_eal -lrte_mbuf -lrte_mempool -lrte_ring -lrte_kvargs -lrte_hash -lrte_cmdline -lrt -lm -ldl -lpthread -Wl,--no-whole-archive")
set(OTHER_LIBS_2 "-Wl,--whole-archive -lrte_pci -lrte_bus_pci -lrte_bus_vdev -lrte_mempool_ring -lnuma -Wl,--no-whole-archive")


target_link_libraries(${PROJECT_NAME} ${DPDK_LIB})
target_link_libraries(${PROJECT_NAME} ${DPDK_LIBS})
target_link_libraries(${PROJECT_NAME} ${OTHER_LIBS})
target_link_libraries(${PROJECT_NAME} ${OTHER_LIBS_2})
target_link_libraries(${PROJECT_NAME} ${PCAPPP_LIBS_DIR})
target_link_libraries(${PROJECT_NAME} Pcap++ Packet++ Common++ pcap)
target_link_libraries(${PROJECT_NAME} pthread dl rt m numa -static-libstdc++)




