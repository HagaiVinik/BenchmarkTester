//
// Created by hagai on 24/11/2020.
//

#ifndef BENCHMARKTESTER_BTTRANSMITTERUDP_HPP
#define BENCHMARKTESTER_BTTRANSMITTERUDP_HPP

#include "BTTransmitter.hpp"


class BTTransmitterUDP : BTTransmitter
{
public:
    BTTransmitterUDP(int buffSize, const std::string &ipAddr, int numOfPackets);

    void startClient();

private:
    unsigned int _serverAddrLen;

    void createSocket();
    void sendNumOfPackets();
    int receiveOkResponse();
    int sendTraffic();
    int receiveThroughputResponse();
};


#endif //BENCHMARKTESTER_BTTRANSMITTERUDP_HPP

