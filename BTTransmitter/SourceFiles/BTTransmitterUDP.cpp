//
// Created by hagai on 24/11/2020.
//

#include "BTTransmitterUDP.hpp"

BTTransmitterUDP::BTTransmitterUDP(int buffSize, const std::string &ipAddr, int numOfPackets) :
        BTTransmitter(ipAddr, buffSize, numOfPackets) ,
        _serverAddrLen(sizeof(_serverAddr))
{
}

void BTTransmitterUDP::createSocket()
{
    static constexpr int failedVal = 0;
    if ((_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        throw std::runtime_error("ERROR: error in createSocket(), socket() failed");
    }

    std::memset(&_serverAddr, 0, sizeof(_serverAddr));

    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(PORT);
    if(_ipAddr == "0.0.0.0")
    {
        _serverAddr.sin_addr.s_addr = INADDR_ANY;
    }
    else if(_ipAddr == "127.0.0.1")
    {
        _serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void BTTransmitterUDP::sendNumOfPackets()
{
    static constexpr int flag = 0;
    std::string strNumOfPackets = std::to_string(_numOfPackets);
    std::string msg = "num_of_packets:";
    msg.append(strNumOfPackets);
    std::cout << "message is: " << msg << std::endl;
    int retVal = sendto(_sock, msg.c_str(), msg.length(), MSG_CONFIRM,
                        (const struct sockaddr *) &_serverAddr,
                        sizeof(_serverAddr));
}

int BTTransmitterUDP::receiveOkResponse()
{
    static constexpr int flag = 0;
    static constexpr int successVal = 0;
    static constexpr int errorVal = -1;
    _valRead = recvfrom(_sock, (char *)_buffer.c_str(), _buffSize,
                              MSG_WAITALL, (struct sockaddr *) &_serverAddr,
                              &_serverAddrLen);

    std::string message(_buffer.substr(0, _valRead));
    _buffer.resize(_buffSize, 1);
    if(_valRead != errorVal)
    {
        if (message == "200OK")
        {
            return successVal;
        }
        else
        {
            std::cout << "ERROR: error in read(),Buffer does not equal 200OK" << std::endl;
        }
    }
    else
    {
        std::cout << "ERROR: error in read(), failed to read response." << std::endl;
    }
    return errorVal;
}

int BTTransmitterUDP::sendTraffic()
{
    std::cout << "Sending traffic....." << std::endl;
    static constexpr int flag = 0;
    for(int i = 0; i < _numOfPackets; ++i)
    {
        sendto(_sock, _buffer.c_str(), _buffer.length(),
               MSG_CONFIRM, (const struct sockaddr *) &_serverAddr,
               sizeof(_serverAddr));
    }
    std::cout << "Finished sending traffic" << std::endl;
    return 0;
}

int BTTransmitterUDP::receiveThroughputResponse()
{
    static constexpr int flag = 0;
    static constexpr int successVal = 0;
    static constexpr int errorVal = -1;
    _valRead = recvfrom(_sock, (char *)_buffer.c_str(), _buffSize,
                              MSG_WAITALL, (struct sockaddr *) &_serverAddr,
                              &_serverAddrLen);

    /*  NOTE: Check message on a string type   */
    std::string throughput(_buffer);
    printResponse(throughput);
    return _valRead;
}

void BTTransmitterUDP::startClient()
{
    try
    {
        createSocket();
    }
    catch (const std::runtime_error &ex)
    {
        std::cout << ex.what() << std::endl;
        return;
    }
    sendNumOfPackets();
    receiveOkResponse();
    sendTraffic();
    receiveThroughputResponse();
}
