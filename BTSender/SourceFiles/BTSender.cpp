//
// Created by hagai on 22/11/2020.
//

#include "../HeaderFiles/BTSender.h"

BTSender::BTSender(const std::string &ipAddr) : ipAddr(ipAddr)
{
}

int BTSender::getNumOfPackets()
{
    return this->numOfPackets;
}

void BTSender::setNumOfPackets(const int &numOfPackets)
{
    this->numOfPackets = numOfPackets;
}

void BTSender::printResponse(const std::string &throughput)
{
    std::cout << "Throghput is: " << throughput << " per second." << std::endl;
}