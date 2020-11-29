/*
//
// Created by hagai on 17/11/2020.
//

#include "PacketStats.h"


PacketStats::PacketStats(){ clear(); }

PacketStats::~PacketStats() {}

void PacketStats::clear()
{
    ethPacketCount = 0;
    ipv4PacketCount = 0;
    ipv6PacketCount = 0;
    tcpPacketCount = 0;
    udpPacketCount = 0;
    tcpPacketCount = 0;
    dnsPacketCount = 0;
    httpPacketCount = 0;
    sslPacketCount = 0;
}

void PacketStats::consumePacket(pcpp::Packet &packet)
{
    std::cout << "Entered consumePackets" << std::endl;
    if (packet.isPacketOfType(pcpp::Ethernet))
        ethPacketCount++;
    if (packet.isPacketOfType(pcpp::IPv4))
        ipv4PacketCount++;
    if (packet.isPacketOfType(pcpp::IPv6))
        ipv6PacketCount++;
    if (packet.isPacketOfType(pcpp::TCP))
        tcpPacketCount++;
    if (packet.isPacketOfType(pcpp::UDP))
        udpPacketCount++;
    if (packet.isPacketOfType(pcpp::HTTP))
        httpPacketCount++;
    if (packet.isPacketOfType(pcpp::SSL))
        sslPacketCount++;
}

void PacketStats::printToConsole()
{
    std::cout << "Ethernet packet count:" << ethPacketCount << std::endl;
    std::cout <<"IPv4 packet count:     " << ipv4PacketCount << std::endl;
    std::cout <<"IPv6 packet count:     " << ipv6PacketCount<< std::endl;
    std::cout <<"TCP packet count:      " << tcpPacketCount<< std::endl;
    std::cout <<"UDP packet count:      " << udpPacketCount<< std::endl;
    std::cout <<"DNS packet count:      " << dnsPacketCount << std::endl;
    std::cout <<"HTTP packet count:     " << httpPacketCount << std::endl;
    std::cout <<"SSL packet count:      " << sslPacketCount << std::endl;
}

*/