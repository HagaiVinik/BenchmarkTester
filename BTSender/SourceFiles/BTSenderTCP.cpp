//
// Created by hagai on 22/11/2020.
//

#include "BTSenderTCP.hpp"


BTSenderTCP::BTSenderTCP(int buffSize, const std::string &ipAddr, int numOfPackets) :
                            BTSender(ipAddr, buffSize, numOfPackets)
{
}

void BTSenderTCP::createSocket()
{
    static constexpr int failedVal = 0;   //retVal of socket()
    if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < failedVal)
    {
        throw std::runtime_error("ERROR: error in createSocket(), socket() failed. ");
    }

    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, _ipAddr.c_str(), &_serverAddr.sin_addr) <= failedVal)
    {
        throw std::runtime_error("ERROR: error in createSocket(), inet_pton() failed, Invalid _address");
    }
}

void BTSenderTCP::connectSocket()
{
    if (connect(_sock, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0)
    {
        throw std::runtime_error("ERROR: error in connectSocket(), connect() failed");
    }
    std::cout << "Connected to server." << std::endl;
}

void BTSenderTCP::sendNumOfPackets()
{
    static constexpr int flag = 0;
    std::string strNumOfPackets = std::to_string(_numOfPackets);
    std::string msg = "num_of_packets:";
    msg.append(strNumOfPackets);
    int retVal = send(_sock, msg.c_str(), msg.length(), flag);
    if (retVal == -1)
        throw std::runtime_error("ERROR: error in sendNumOfPackets(), send() failed.");
}

int BTSenderTCP::receiveOkResponse()
{
    static constexpr int flag = 0;
    static constexpr int successVal = 0;
    static constexpr int errorVal = -1;

    _valRead = recv(_sock, (char *)_buffer.c_str(), _buffer.length(), flag);
    std::string message(_buffer.substr(0, _valRead));
    _buffer.resize(_buffSize, 1);
    if(_valRead != errorVal)
    {
        if(message == "200OK")
        {
            return successVal;
        }
        else {
            std::cout << "ERROR: error in read(), Buffer does not equal 200OK." << std::endl;
        }
    } else {
        std::cout << "ERROR: error in read(), failed to read response." << std::endl;
    }
    return errorVal;
}

int BTSenderTCP::sendTraffic()
{
    std::cout << "Sending traffic....." << std::endl;
    static constexpr int flag = 0;
    static constexpr int successVal = 0;
    static constexpr int errorVal = -1;
    int retVal = 0;

    for(int i = 0; i < _numOfPackets; ++i)
    {
        retVal = send(_sock, _buffer.c_str(), _buffer.length(), flag);
        if(retVal == errorVal)
            return retVal;
    }
    std::cout << "Finished sending traffic. " << std::endl;
    return successVal;
}

int BTSenderTCP::receiveThroughputResponse()
{
    static constexpr int flag = 0;
    _valRead = recv(_sock, (char *)_buffer.c_str(), _buffer.length(), flag);
    std::string throughput(_buffer);
    printResponse(_buffer);
    return _valRead;
}

void BTSenderTCP::startClient()
{
    static constexpr int successVal = 0;
    static constexpr int errorVal = -1;
    int retVal;
    try{
        createSocket();
        connectSocket();
        sendNumOfPackets();
    } catch (const std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
        return;
    }

    retVal = receiveOkResponse();
    if(retVal == successVal)
    {
        sendTraffic();
        retVal = receiveThroughputResponse();
    }
}

