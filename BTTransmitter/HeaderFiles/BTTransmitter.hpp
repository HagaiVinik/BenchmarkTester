//
// Created by hagai on 22/11/2020.
//

#ifndef BENCHMARKTESTER_BTTRANSMITTER_HPP
#define BENCHMARKTESTER_BTTRANSMITTER_HPP

#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class BTTransmitter
{
public:
    BTTransmitter(const std::string &ipAddr, int buffSize, int numOfPackets);
    void printResponse(const std::string &throughput);

protected:
    int _sock;
    int _valRead;
    sockaddr_in _serverAddr;

    int _buffSize;
    std::string _buffer;
    static constexpr int PORT = 7600;
    int _numOfPackets;
    std::string _ipAddr;
};

#endif //BENCHMARKTESTER_BTTRANSMITTER_HPP

