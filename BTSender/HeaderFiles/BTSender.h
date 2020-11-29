//
// Created by hagai on 22/11/2020.
//

#ifndef BENCHMARKTESTER_BTSENDER_H
#define BENCHMARKTESTER_BTSENDER_H

#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class BTSender {
protected:
    const int port = 7600;
    int numOfPackets;
    std::string ipAddr;
public:
    BTSender(const std::string &ipAddr);
    ~BTSender()= default;
    void setNumOfPackets(const int &numOfPackets=1024);
    int getNumOfPackets();
    void printResponse(const std::string &throughput);
};

#endif //BENCHMARKTESTER_BTSENDER_H
