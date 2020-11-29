/*

//
// Created by hagai on 17/11/2020.
//

#include "BTServer.h"

BTServer::BTServer() : isCapturing(false) {}

void BTServer::initialize(const std::string &ipAddr)
{
    std::cout << "DEBUG: entered initialize" << std::endl;

    this->interfaceIPAddr = ipAddr;
    this->device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIPAddr.c_str());
    if(device == NULL)
    {
        std::cout << "Cannot find interface with IPv4 address of " << interfaceIPAddr.c_str() << std::endl ;
        throw std::exception();
    }
}

void BTServer::printDeviceInfo()
{
    std::cout << "Interface info:" << std::endl;
// get interface name
    std::cout << "   Interface name:        " << device->getName() << std::endl;
// get interface description
    std::cout << "   Interface description: " << device->getDesc() << std::endl;
// get interface MAC address
    std::cout << "   MAC address:           " << device->getMacAddress().toString().c_str() << std::endl;
// get default gateway for interface
    std::cout << "   Default gateway:       " << device->getDefaultGateway().toString().c_str()<< std::endl;
// get interface MTU
    std::cout << "   Interface MTU:         " << device->getMtu()<< std::endl;
// get DNS server if defined for this interface
    if (device->getDnsServers().size() > 0)
        std::cout << "   DNS server:            " << device->getDnsServers().at(0).toString().c_str()<< std::endl;
}

void BTServer::printPacketsData()
{
    stats.printToConsole();
}

void BTServer::openDevice()
{
    if(!this->device->open())
    {
        throw std::runtime_error("Cannot open device");
    }
}

void BTServer::onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    std::cout << "Entered onPacketArrives " << std::endl;
    // extract the stats object form the cookie
    PacketStats* stats = (PacketStats*)cookie;

    // parsed the raw packet
    pcpp::Packet parsedPacket(packet);

    // collect stats from packet
    stats->consumePacket(parsedPacket);
}

void BTServer::startCapture()
{
    if(!isServerCapturing())
    {
        device->startCapture(onPacketArrives, &stats);
        isCapturing = true;
    }
    else{
        std::cout << "Server is already capturing" << std::endl;
    }
}

void BTServer::stopCapture()
{
    device->stopCapture();
}

void BTServer::sendResponse(pcpp::RawPacket packet)
{
    if(!device->sendPacket(packet))
        throw std::runtime_error("ERROR: couldn't send response");
}

bool BTServer::isServerCapturing()
{
    return isCapturing;
}
*/