//
// Created by hagai on 02/12/2020.
//

#ifndef BENCHMARKTESTER_BTPCPPDPDK_HPP
#define BENCHMARKTESTER_BTPCPPDPDK_HPP


#include <vector>
#include <unistd.h>
#include <sstream>
#include <cstdlib>

#include "SystemUtils.h"
#include "DpdkDeviceList.h"
#include "TablePrinter.h"
#include "SystemUtils.h"

#include "AppWorkerThread.hpp"
#include "BTReceiver.hpp"


class BTpcppDPDK {
public:
    BTpcppDPDK(const int buffSize, const std::string &ipAddr, const int role);
    ~BTpcppDPDK() = default;

    static constexpr auto PORT = 7600;
    static constexpr auto MBUF_POOL_SIZE = 4095;
    static constexpr auto DEVICE_ID_1 = 0;
    static constexpr auto RECEIVER = 1;
    static constexpr auto TRANSMITTER = 2;

    void initializeDpdk();
    void startServer();
    int findDpdkDevices();
    int openDpdkDevices();
    void setWorker();
    long double calculateThroughputVal(long timeInMiliSeconds);
    void startWorkerThreads();
    void registerToEvent();

private:
    int _role;
    std::string _ipAddr;
    int _buffSize;
    int _numOfPackets;
    pcpp::DpdkDevice* _device;
    std::vector<pcpp::DpdkWorkerThread*> _workers;
    bool _isRunning = true;
    pcpp::CoreMask _coreMaskToUse;
    pcpp::Packet* _packetPtr;
    long _timeInMilliSeconds = 0;

    void setFilter();

    /* Callback function */
    static void onApplicationInterrupted(void* cookie);
};


#endif //BENCHMARKTESTER_BTPCPPDPDK_HPP


// static void onPacketArrives();
// void sendThroughput();
// long double calculateThroughputVal(long timeInMiliSeconds);


