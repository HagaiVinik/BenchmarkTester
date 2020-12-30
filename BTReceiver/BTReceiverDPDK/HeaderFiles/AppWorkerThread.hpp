//
// Created by hagai on 02/12/2020.
//

#ifndef BENCHMARKTESTER_APPWORKERTHREAD_HPP
#define BENCHMARKTESTER_APPWORKERTHREAD_HPP

#pragma once

#include <iostream>

#include "PacketUtils.h"
#include "DpdkDevice.h"
#include "DpdkDeviceList.h"
#include "PcapFileDevice.h"

#include "Common.hpp"

static constexpr auto DEFAULT_CAPACITY = 1024;

class AppWorkerThread : public pcpp::DpdkWorkerThread{
private:
    pcpp::DpdkDevice* _device;
    bool _stop;
    uint32_t _coreId;

    uint16_t _role;
    const int _buffSize = 1024;
    int _numOfPackets = 1024;
    pcpp::Packet* _packetPtr;

    //static constexpr auto DEFAULT_CAPACITY = 1024;
    static constexpr auto RECEIVER = 0;
    static constexpr auto SENDER = 0;

public:
    AppWorkerThread(pcpp::DpdkDevice* device,
                    uint16_t _role,
                    pcpp::Packet* packetPtr,
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
};


#endif //BENCHMARKTESTER_APPWORKERTHREAD_HPP
