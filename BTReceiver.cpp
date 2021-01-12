//
// Created by hagai on 18/11/2020.
//

#include "BTReceiver.hpp"

BTReceiver::BTReceiver(const std::string &ipAddr, int buffSize)
:   _buffSize(buffSize),
    _ipAddr(ipAddr)
{
    _buffer.resize(_buffSize, 0);
}

void BTReceiver::setNumOfPackets(int numOfPackets)
{
    _numOfPackets = numOfPackets;
}

void BTReceiver::computeThroughput(long double throughput)
{
    double result = 0.0;
    if (throughput >= GB_SIZE)
    {
        result = throughput / GB_SIZE;
        _throughputType = "GB";
    }
    else if (throughput >= MB_SIZE)
    {
        result = throughput / MB_SIZE;
        _throughputType = "MB";
    }
    else if(throughput >= KB_SIZE)
    {
        result = throughput / KB_SIZE;
        _throughputType = "KB";
    }
    _throughputVal = result;
}
