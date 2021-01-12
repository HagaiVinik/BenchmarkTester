//
// Created by hagai on 22/11/2020.
//

#ifndef BENCHMARKTESTER_BTTRANSMITTERTCP_HPP
#define BENCHMARKTESTER_BTTRANSMITTERTCP_HPP

#include "BTTransmitter.hpp"


class BTTransmitterTCP : BTTransmitter
{
public:
    BTTransmitterTCP(int buffSize, const std::string &ipAddr, int numOfPackets);
    void startClient();

private:
    void sendNumOfPackets();
    void createSocket();
    void connectSocket();
    int sendTraffic();
    int receiveOkResponse();
    int receiveThroughputResponse();

};


#endif //BENCHMARKTESTER_BTTRANSMITTERTCP_HPP

