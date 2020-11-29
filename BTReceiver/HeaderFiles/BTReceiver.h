//
// Created by hagai on 18/11/2020.
//

#ifndef BENCHMARKTESTER_BTRECEIVER_H
#define BENCHMARKTESTER_BTRECEIVER_H

#include <iostream>
#include <cstring>
#include <cmath>
#include <chrono>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


class BTReceiver{
public:
    BTReceiver(const std::string &ipAddr, const int &buffSize);
    ~BTReceiver() = default;
    int getPort();
    long double throughputVal;
    std::string throughputType;
    std::string getIpAddr();

protected:
    int valRead;
    int opt;
    int buffSize;
    std::string buffer;
    int serverFd;

    const int port = 7600;
    std::string ipAddr;
    int numOfPackets;
    void setNumOfPackets(const int &numOfPackets);
    int getNumOfPackets();
    void computeThroughput(const long double &throughput);

    /*
     * Note: Consider declaring virtual functions here.
     */
};


#endif //BENCHMARKTESTER_BTRECEIVER_H

/*
 * virtual doBT
 */