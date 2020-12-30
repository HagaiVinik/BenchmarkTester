//
// Created by hagai on 02/12/2020.
//

#ifndef BENCHMARKTESTER_BTRECEIVERDPDK_HPP
#define BENCHMARKTESTER_BTRECEIVERDPDK_HPP


#include <vector>
#include <unistd.h>
#include <sstream>
#include <cstdlib>

#include "SystemUtils.h"
#include "DpdkDeviceList.h"
#include "TablePrinter.h"
#include "SystemUtils.h"

#include "Packet.h"
#include "EthLayer.h"
#include "VlanLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include <PayloadLayer.h>
#include "UdpLayer.h"
#include "DnsLayer.h"
#include "PcapFileDevice.h"

#include "AppWorkerThread.hpp"
#include "BTReceiver.hpp"


class BTReceiverDPDK : public BTReceiver {
public:
    BTReceiverDPDK(const int &buffSize, const std::string &ipAddr);
    ~BTReceiverDPDK() = default;

    static constexpr auto MBUF_POOL_SIZE = 4095;
    static constexpr auto DEVICE_ID_1 = 4095;

    void startServer();
    int findDpdkDevices();
    int openDpdkDevices();
    void setWorker();
    void startWorkerThreads();
    void registerToEvent();

private:
    pcpp::DpdkDevice* _device;
    std::vector<pcpp::DpdkWorkerThread*> _workers;
    bool _isRunning = true;
    pcpp::CoreMask _coreMaskToUse;
    pcpp::Packet* _packetPtr;

    void setFilter();
    std::string getLocalIPAddress();
    void craftPacket();

    /* Callback function */
    static void onApplicationInterrupted(void* cookie);
};


#endif //BENCHMARKTESTER_BTRECEIVERDPDK_HPP


// static void onPacketArrives();
// void sendThroughput();
// long double calculateThroughputVal(long timeInMiliSeconds);


