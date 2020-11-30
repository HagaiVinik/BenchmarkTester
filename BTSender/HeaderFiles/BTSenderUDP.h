//
// Created by hagai on 24/11/2020.
//

#ifndef BENCHMARKTESTER_BTSENDERUDP_H
#define BENCHMARKTESTER_BTSENDERUDP_H

#include "BTSender.h"


class BTSenderUDP : BTSender {
public:
    BTSenderUDP(const int &buffSize, const std::string &ipAddr, const int &numOfPackets);
    ~BTSenderUDP() = default;

    void startClient();

private:
    unsigned int serverAddrLen = sizeof(serverAddr);

    /* Hagai:
     * Note: remember to fix buffer an buff size. (size is determind by the client).
     * */

    void createSocket();
    void sendNumOfPackets();
    int receiveOkResponse();
    int sendTraffic();
    int receiveThroughputResponse();
};


#endif //BENCHMARKTESTER_BTSENDERUDP_H

