//
// Created by hagai on 22/11/2020.
//

#include "../HeaderFiles/BTSender.h"

BTSender::BTSender(const std::string &ipAddr, const int &buffSize, const int &numOfPackets)
{
    this->ipAddr = ipAddr;
    this->buffSize = buffSize;
    this->buffer.resize(this->buffSize, 0);
    this->numOfPackets = numOfPackets;
}

void BTSender::printResponse(const std::string &throughput)
{
    std::cout << "Throghput is: " << throughput << " per second." << std::endl;
}