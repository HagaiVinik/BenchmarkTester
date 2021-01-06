//
// Created by hagai on 22/11/2020.
//

#ifndef BENCHMARKTESTER_BTSENDERTCP_HPP
#define BENCHMARKTESTER_BTSENDERTCP_HPP

#include "BTSender.hpp"


class BTSenderTCP : BTSender
{
public:
    BTSenderTCP(int buffSize, const std::string &ipAddr, int numOfPackets);
    void startClient();

private:
    void sendNumOfPackets();
    void createSocket();
    void connectSocket();
    int sendTraffic();
    int receiveOkResponse();
    int receiveThroughputResponse();

};


#endif //BENCHMARKTESTER_BTSENDERTCP_HPP

