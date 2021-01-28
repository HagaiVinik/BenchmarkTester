//
// Created by hagai on 24/11/2020.
//

#include "BTReceiverUDP.hpp"


BTReceiverUDP::BTReceiverUDP(int buffSize, const std::string &ipAddr) : BTReceiver(ipAddr, buffSize)
{
}

void BTReceiverUDP::createSocket()
{
    static constexpr int failedVal = 0;
    if ((_serverFd = socket(AF_INET, SOCK_DGRAM, 0)) < failedVal )
    {
        throw std::runtime_error("ERROR: error in Socket(), creation error");
    }
    memset(&_serverAddr, 0, sizeof(_serverAddr));
    memset(&_clientAddr, 0, sizeof(_clientAddr));
    _serverAddr.sin_family = AF_INET;
    if(_ipAddr == ADDR_ANY)
    {
        _serverAddr.sin_addr.s_addr = INADDR_ANY;
    }
    else if(_ipAddr == ADDR_LOOPBACK)
    {
        _serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    _serverAddr.sin_port = htons(PORT);
}

void BTReceiverUDP::setTimeOut()
{
    // setting timeout for recvfrom()
    timeval tv;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int retVal = setsockopt(_serverFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if(retVal < 0)
    {
        throw std::runtime_error("ERROR: error in setTomeOut(), setsockopt() failed.");
    }
}

void BTReceiverUDP::bindSocket()
{
    if (bind(_serverFd, (const struct sockaddr *)&_serverAddr,
              sizeof(_serverAddr)) < 0 )
    {
        throw std::runtime_error("ERROR: error in bindSocket(), bind() failed");
    }
    std::cout << "Socket is Binding on: '" << _ipAddr << "', PORT:" << PORT << " (UDP)" <<std::endl;
}

int BTReceiverUDP::receiveNumOfPackets()
{;
    static constexpr int successVal = 0;
    static constexpr int errorVal = -1;
    uint32_t len = sizeof(_clientAddr);
    _valRead = recvfrom(_serverFd, (char *)_buffer.c_str(), _buffSize,
                              MSG_WAITALL, ( struct sockaddr *) &_clientAddr,
                              &len);
    _buffer.at(_valRead) = 0;
    if(_valRead != errorVal)
    {
        std::string strBuff(_buffer);
        std::string strNumOfPackets = strBuff.substr( strBuff.find(':')+1, strBuff.find("/0")-1);
        int numOfPackets = std::stoi(strNumOfPackets);
        setNumOfPackets(numOfPackets);
        std::cout << "Num of packets:" << numOfPackets << std::endl;
        return successVal;
    }
    else
    {
        std::cout << "ERROR: error in recvfrom(), failed to read response." << std::endl;
        return errorVal;
    }
}

void BTReceiverUDP::sendResponseOK()
{
    const std::string okMessage = "200OK";
    sendto(_serverFd, okMessage.c_str(), okMessage.length(),
           MSG_CONFIRM, (const struct sockaddr *) &_clientAddr,
           sizeof(_clientAddr));
}

void BTReceiverUDP::handleTraffic()
{
    static constexpr int errorVal = -1;

    uint32_t len = sizeof(_clientAddr);

    double successInPercents = 0.0;
    int numOfPacketsLost = 0;
    uint64_t numOfPacketsArrived = 0;

    std::cout << "computing throughput....." << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < _numOfPackets; ++i)
    {
        _valRead = errorVal;
        _valRead = recvfrom(_serverFd, (char *)_buffer.c_str(),
                                  (sizeof(_buffer.c_str())), MSG_WAITALL,
                                  ( struct sockaddr *) &_clientAddr, &len);

        if(_valRead == errorVal)
        {
            break;
        }
        if (_valRead != errorVal)
        {
            ++numOfPacketsArrived;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto timeInMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    double accurate_time = static_cast<double>(timeInMilliSeconds) / 1000.00;
    if(numOfPacketsArrived < _numOfPackets)
        accurate_time -= 1.00;      // timeout set t0 1sec, if all packets arrived,
                                    //                  there is no need to calculate timeout.

    /*  Calculate throughput.    */
    long double throughput = (_numOfPackets * _buffSize) / (double)accurate_time;
    computeThroughput(throughput);

    /*   Calculate success in percents and count how much packets had lost.   */
    successInPercents = ((double)numOfPacketsArrived / (double)_numOfPackets) * 100.0;
    numOfPacketsLost = _numOfPackets - numOfPacketsArrived;

    sendThroughput(successInPercents, numOfPacketsLost);
}

void BTReceiverUDP::sendThroughput(double successInPercents, int numOfPacketsLost)
{
    std::string strThroughputVal = std::to_string(_throughputVal);
    std::string strSuccessInPercents = std::to_string(successInPercents);
    std::string strNumOfPacketsLost = std::to_string(numOfPacketsLost);
    std::string finalThroughputMsg = strThroughputVal + _throughputType +
                                     "(" + strSuccessInPercents + "%, lost: " +
                                     strNumOfPacketsLost + ")";

    std::cout << "Throughput is : " << finalThroughputMsg << " per second. " << std::endl;
    sendto(_serverFd, finalThroughputMsg.c_str(), finalThroughputMsg.length(),
           MSG_CONFIRM, (const struct sockaddr *) &_clientAddr,
           sizeof(_clientAddr));
    std::cout << "Sent to client." << std::endl;
}

void BTReceiverUDP::startServer(int maxConnectionRequests)
{
    static constexpr int failedVal = -1;
    int retVal;
    try
    {
        createSocket();
        bindSocket();
    } catch (const std::runtime_error &ex){
        std::cout << ex.what() << std::endl;
        return;
    }

    retVal = receiveNumOfPackets();
    if(retVal == failedVal)
    {
        std::cout << "Failed receiving number of packets." << std::endl;
        return;
    }
    sendResponseOK();
    setTimeOut();
    handleTraffic();
}

