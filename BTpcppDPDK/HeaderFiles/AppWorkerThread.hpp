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
    std::string _ipAddr;

    long double _throughputVal;
    std::string _throughputType;

public:
    AppWorkerThread(pcpp::DpdkDevice* device,
                    uint16_t _role,
                    const std::string &ipAddr,
                    int buffSize=DEFAULT_CAPACITY,
                    int numOfPackets=DEFAULT_CAPACITY);

    ~AppWorkerThread() override = default;

    // implement abstract method
    // start running the worker thread
    bool run(uint32_t coreId) override ;

    // ask the worker thread to stop
    void stop () override ;

    // get worker thread core ID
    uint32_t getCoreId() const override ;

    static constexpr auto RECEIVER = 1;
    static constexpr auto TRANSMITTER = 2;
    static constexpr auto DEFAULT_CAPACITY = 1024;
    static constexpr auto PORT = 7600;
    static constexpr auto KB_SIZE = 1024.0;
    static constexpr auto MB_SIZE = KB_SIZE * 1024.0;
    static constexpr auto GB_SIZE = MB_SIZE * 1024.0;

    void craftPacket();
    void computeThroughput(long timeInMiliSeconds);
    void sendThroughput();
    void receiveThroughput();


};


#endif //BENCHMARKTESTER_APPWORKERTHREAD_HPP
