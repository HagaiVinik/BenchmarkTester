//
// Created by hagai on 18/11/2020.
//

#include "../HeaderFiles/BTReceiver.h"

BTReceiver::BTReceiver(const std::string &ipAddr, const int &buffSize)
{
    this->throughputVal = 0.0;
    this->throughputType = "";
    this->opt = 1;
    this->buffSize = buffSize;
    this->ipAddr = ipAddr;
    this->buffer.resize(this->buffSize, 0);
    //this->buffer = new char[this->buffSize]();
    //memset(this->buffer, 0, buffSize);
}

std::string BTReceiver::getIpAddr()
{
    return ipAddr;
}

int BTReceiver::getPort()
{
    return port;
}

int BTReceiver::getNumOfPackets()
{
    return this->numOfPackets;
}

void BTReceiver::setNumOfPackets(const int &numOfPackets)
{
    this->numOfPackets = numOfPackets;
}

void BTReceiver::computeThroughput(const long double &throughput)
{
    double result = 0.0;
    if (throughput >= 1073741824.0)
    {
        result = throughput / 1073741824.0;
        this->throughputType = "GB";
    }
    else if (throughput >= 1048576.0)
    {
        result = throughput / 1048576.0;
        this->throughputType = "MB";
    }
    else if(throughput >= 1024.0)
    {
        result = throughput / 1024.0;
        this->throughputType = "KB";
    }
    this->throughputVal = result;
}
