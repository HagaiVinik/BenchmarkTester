//
// Created by hagai on 22/11/2020.
//

#include "BTSender.hpp"

BTSender::BTSender(const std::string &ipAddr, const int &buffSize, const int &numOfPackets)
: _ipAddr(ipAddr), _buffSize(buffSize), _numOfPackets(numOfPackets)
{
    _buffer.resize(_buffSize, 0);
}

void BTSender::printResponse(const std::string &throughput)
{
    std::cout << "Throghput is: " << throughput << " per second." << std::endl;
}
