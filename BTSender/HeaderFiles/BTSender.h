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
public:
    BTSender(const std::string &ipAddr, const int &buffSize, const int &numOfPackets);
    ~BTSender()= default;
    void printResponse(const std::string &throughput);

protected:
    int sock;
    int valRead;
    sockaddr_in serverAddr;

    int buffSize;
    std::string buffer;
    const int port = 7600;
    int numOfPackets;
    std::string ipAddr;

    /*
     * Note: Consider declaring virtual functions here.
     */
};

#endif //BENCHMARKTESTER_BTSENDER_H
