//
// Created by hagai on 02/12/2020.
//

#ifndef BENCHMARKTESTER_APPWORKERTHREAD_HPP
#define BENCHMARKTESTER_APPWORKERTHREAD_HPP

#pragma once

#include <iostream>
#include <chrono>

#include "PacketUtils.h"
#include "DpdkDevice.h"
#include "DpdkDeviceList.h"
#include "PcapFileDevice.h"

#include "Packet.h"
#include "EthLayer.h"
#include "VlanLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include <PayloadLayer.h>
#include "UdpLayer.h"
#include "DnsLayer.h"
#include "PcapFileDevice.h"

#include "Common.hpp"

class AppWorkerThread : public pcpp::DpdkWorkerThread{
private:
    pcpp::DpdkDevice* _device;
    bool _stop;
    uint32_t _coreId;

    uint16_t _role;
    const int _buffSize;
    const int _numOfPackets;
    pcpp::Packet* _packetPtr;
    long double _throughput;
    std::string _ipAddr;

public:
    AppWorkerThread(pcpp::DpdkDevice* device,
                    uint16_t _role,
                    std::string ipAddr,
                    int buffSize=DEFAULT_CAPACITY,
                    int numOfPackets=DEFAULT_CAPACITY);

    ~AppWorkerThread() { /*Do nothing*/ }

    // implement abstract method
    // start running the worker thread
    bool run(uint32_t coreId) ;

    // ask the worker thread to stop
    void stop () ;

    // get worker thread core ID
    uint32_t getCoreId() const ;

    static constexpr auto RECEIVER = 0;
    static constexpr auto TRANSMITTER = 0;
    static constexpr auto DEFAULT_CAPACITY = 1024;
    static constexpr auto PORT = 7600;

    void craftPacket();

};


#endif //BENCHMARKTESTER_APPWORKERTHREAD_HPP
