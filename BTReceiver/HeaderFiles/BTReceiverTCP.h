//
// Created by hagai on 19/11/2020.
//

#ifndef BENCHMARKTESTER_BTRECEIVERTCP_H
#define BENCHMARKTESTER_BTRECEIVERTCP_H


#include "BTReceiver.h"


class BTReceiverTCP : public BTReceiver {
public:
    BTReceiverTCP(const int &buffSize, const std::string &ipAddr);
    ~BTReceiverTCP() = default;
    void startServer(const int &maxConnectionRequests=1);

private:
    int clientSocket;
    sockaddr_in address;
    int addrLen = sizeof(address);

    void createSocket();
    void bindSocket();
    void listenForConnection(const int &maxConnectionRequests=1);
    int receiveNumOfPackets();
    void sendResponseOK();
    void handleTraffic();
    void sendThroughput();
};


#endif //BENCHMARKTESTER_BTRECEIVERTCP_H

/*
 * startBT instead of all public methods.
 */