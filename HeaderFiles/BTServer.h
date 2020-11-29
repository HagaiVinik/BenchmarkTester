/*
//
// Created by hagai on 17/11/2020.
//

#ifndef BENCHMARKTESTER_BTSERVER_H
#define BENCHMARKTESTER_BTSERVER_H


#include <iostream>

#include "PcapLiveDeviceList.h"

#include "PacketStats.h"

class BTServer {
public:
    BTServer();
    ~BTServer() = default;

    void initialize(const std::string &ipAddr);
    void openDevice();
    void startCapture();
    void stopCapture();

    void sendResponse(pcpp::RawPacket packet);
    void printDeviceInfo();
    void printPacketsData();

    static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie);

    bool isServerCapturing();


private:
    pcpp::PcapLiveDevice *device;
    //pcpp::PortFilter portFilter;
    pcpp::ProtoFilter protocolFilter;
    //pcpp::Packet ;


    std::string interfaceIPAddr;
    bool isCapturing;
    PacketStats stats;

};


#endif //BENCHMARKTESTER_BTSERVER_H
*/