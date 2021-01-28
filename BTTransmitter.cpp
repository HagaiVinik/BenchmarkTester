//
// Created by hagai on 22/11/2020.
//

#include "BTTransmitter.hpp"

BTTransmitter::BTTransmitter(const std::string &ipAddr, int buffSize, int numOfPackets)
: _ipAddr(ipAddr), _buffSize(buffSize), _numOfPackets(numOfPackets)
{
    _buffer.resize(_buffSize, 0);
}

void BTTransmitter::printResponse(const std::string &throughput)
{
    std::cout << "Throghput is: " << throughput << " per second." << std::endl;
}
