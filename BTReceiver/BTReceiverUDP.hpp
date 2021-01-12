//
// Created by hagai on 24/11/2020.
//

#ifndef BENCHMARKTESTER_BTRECEIVERUDP_HPP
#define BENCHMARKTESTER_BTRECEIVERUDP_HPP


#include "../BTReceiver.hpp"


class BTReceiverUDP : public BTReceiver
{
public:
    BTReceiverUDP(int buff_size,const std::string &ipAddr);
    void startServer(const int maxConnectionRequests=1);

private:
    sockaddr_in _serverAddr;
    sockaddr_in _clientAddr;

    void createSocket();
    void setTimeOut();
    void bindSocket();
    int receiveNumOfPackets();
    void sendResponseOK();
    void handleTraffic();
    void sendThroughput(double successInPercents, int numOfPacketsLost);
};


#endif //BENCHMARKTESTER_BTRECEIVERUDP_HPP

