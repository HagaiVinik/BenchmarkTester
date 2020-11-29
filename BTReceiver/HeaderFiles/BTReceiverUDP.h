//
// Created by hagai on 24/11/2020.
//

#ifndef BENCHMARKTESTER_BTRECEIVERUDP_H
#define BENCHMARKTESTER_BTRECEIVERUDP_H


#include "BTReceiver.h"


class BTReceiverUDP : public BTReceiver {
public:
    BTReceiverUDP(const int &buff_size,const std::string &ipAddr);
    ~BTReceiverUDP() = default;
    void startServer(const int &maxConnectionRequests=1);
private:
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;

    void createSocket(); // done.
    void setTimeOut();
    void bindSocket();  //done.
    int receiveNumOfPackets(); //done.
    void sendResponseOK(); //done.
    void handleTraffic(); //done.
    void sendThroughput(); //done.

};


#endif //BENCHMARKTESTER_BTRECEIVERUDP_H
