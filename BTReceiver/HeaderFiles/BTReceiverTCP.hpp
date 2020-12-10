//
// Created by hagai on 19/11/2020.
//

#ifndef BENCHMARKTESTER_BTRECEIVERTCP_HPP
#define BENCHMARKTESTER_BTRECEIVERTCP_HPP


#include "BTReceiver.hpp"


class BTReceiverTCP : public BTReceiver {
public:
    BTReceiverTCP(const int &buffSize, const std::string &ipAddr);
    ~BTReceiverTCP() = default;
    void startServer(const int &maxConnectionRequests=1);

private:
    int _clientSocket;
    sockaddr_in _address;

    const int ADDR_LEN = sizeof(_address);
    void createSocket();
    void bindSocket();
    void listenForConnection(const int maxConnectionRequests=1);
    int receiveNumOfPackets();
    void sendResponseOK();
    void handleTraffic();
    void sendThroughput();
    long double calculateThroughputVal(long timeInMiliSeconds);
};


#endif //BENCHMARKTESTER_BTRECEIVERTCP_HPP



