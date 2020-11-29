//
// Created by hagai on 22/11/2020.
//

#ifndef BENCHMARKTESTER_BTSENDERTCP_H
#define BENCHMARKTESTER_BTSENDERTCP_H

#include "BTSender.h"


class BTSenderTCP : BTSender {
public:
    BTSenderTCP(const int &buffSize, const std::string &ipAddr, const int &numOfPackets);
    ~BTSenderTCP() = default;

    void startClient();

private:
    /* Hagai:
     * Note: remember to fix buffer an buff size. (size is determind by the client).
     * */
    void sendNumOfPackets();
    void createSocket();
    void connectSocket();
    int sendTraffic();
    int receiveOkResponse();
    int receiveThroughputResponse();

};


#endif //BENCHMARKTESTER_BTSENDERTCP_H
