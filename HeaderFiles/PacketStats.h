/*
//
// Created by hagai on 17/11/2020.
//

#ifndef BENCHMARKTESTER_PACKETSTATS_H
#define BENCHMARKTESTER_PACKETSTATS_H

#include <iostream>
#include "PcapLiveDeviceList.h"


class PacketStats{
public:
    PacketStats();
    ~PacketStats();
    void clear();
    void consumePacket(pcpp::Packet& packet);
    void printToConsole();

private:
    int ethPacketCount;
    int ipv4PacketCount;
    int ipv6PacketCount;
    int tcpPacketCount;
    int udpPacketCount;
    int dnsPacketCount;
    int httpPacketCount;
    int sslPacketCount;
};


#endif //BENCHMARKTESTER_PACKETSTATS_H
*/