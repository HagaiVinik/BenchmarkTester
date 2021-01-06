//
// Created by hagai on 24/11/2020.
//

#ifndef BENCHMARKTESTER_BTSENDERUDP_HPP
#define BENCHMARKTESTER_BTSENDERUDP_HPP

#include "BTSender.hpp"


class BTSenderUDP : BTSender
{
public:
    BTSenderUDP(int buffSize, const std::string &ipAddr, int numOfPackets);

    void startClient();

private:
    unsigned int _serverAddrLen;

    void createSocket();
    void sendNumOfPackets();
    int receiveOkResponse();
    int sendTraffic();
    int receiveThroughputResponse();
};


#endif //BENCHMARKTESTER_BTSENDERUDP_HPP

