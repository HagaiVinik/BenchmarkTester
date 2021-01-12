//
// Created by hagai on 18/11/2020.
//

#ifndef BENCHMARKTESTER_BTRECEIVER_HPP
#define BENCHMARKTESTER_BTRECEIVER_HPP

#include <iostream>
#include <cstring>
#include <cmath>
#include <chrono>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


class BTReceiver
{
public:
    BTReceiver(const std::string &ipAddr, int buffSize);

    long double _throughputVal = 0.0;
    std::string _throughputType = "";

    static constexpr auto KB_SIZE = 1024.0;
    static constexpr auto MB_SIZE = KB_SIZE * 1024.0;
    static constexpr auto GB_SIZE = MB_SIZE * 1024.0;

    static constexpr auto ADDR_ANY = "0.0.0.0";
    static constexpr auto ADDR_LOOPBACK = "127.0.0.1";

protected:
    int _valRead;
    int _opt = 1;
    int _buffSize;
    std::string _buffer;
    int _serverFd;

    static constexpr uint16_t PORT = 7600;

    std::string _ipAddr;
    int _numOfPackets;

    void setNumOfPackets(int numOfPackets);
    void computeThroughput(long double throughput);

};


#endif //BENCHMARKTESTER_BTRECEIVER_HPP
