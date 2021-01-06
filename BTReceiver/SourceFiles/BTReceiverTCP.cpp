//
// Created by Hagai on 19/11/2020.
//

#include "BTReceiverTCP.hpp"


BTReceiverTCP::BTReceiverTCP(int buffSize, const std::string &ipAddr) :
                            BTReceiver(ipAddr, buffSize)
{

}

void BTReceiverTCP::createSocket()
{
    static constexpr int failedVal = 0;   //retVal of socket()

    if((_serverFd = socket(AF_INET, SOCK_STREAM, 0)) == failedVal)
    {
        throw std::runtime_error("ERROR: Socket() failed. ");
    }
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &_opt, sizeof(_opt)))
    {
        throw std::runtime_error("ERROR:: setSockOpt() failed");
    }

    /** Note: supports binding to loopBackConnetion and anyConnection only*/
    _address.sin_family = AF_INET;
    if(_ipAddr == ADDR_ANY)
    {
        _address.sin_addr.s_addr = INADDR_ANY;
    }
    else if(_ipAddr == ADDR_LOOPBACK)
    {
        _address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    _address.sin_port = htons(PORT);
}

void BTReceiverTCP::bindSocket()
{
    static constexpr int successVal = 0;
    if (bind(_serverFd, (struct sockaddr *)&_address, sizeof(_address)) < successVal)
    {
        throw std::runtime_error("ERROR: error in bindSocket(), bind() failed. ");
    }
}

void BTReceiverTCP::listenForConnection(int maxConnectionRequests)
{
    static constexpr int successVal = 0;
    if (listen(_serverFd, maxConnectionRequests) < successVal)
    {
        throw std::runtime_error("ERROR: error in listenForConnection(), listen() failed. ");
    }
    std::cout << "Socket is listening on: '" << _ipAddr << "', PORT:" << PORT << std::endl;
    if ((_clientSocket = accept(_serverFd, (struct sockaddr *)&_address,
                                      (socklen_t*)&ADDR_LEN)) < successVal)
    {
        throw std::runtime_error("ERROR: error in listenForConnection(), listen() failed. ");
    }
    std::cout << "client connected. " << std::endl;
}

int BTReceiverTCP::receiveNumOfPackets()
{
    static constexpr int flag = 0;
    static constexpr int successVal = 1;
    static constexpr int errorVal = -1;

    _valRead = recv(_clientSocket, (char *)_buffer.c_str(), _buffSize, flag);
    if(_valRead != errorVal)
    {
        if(_valRead == 0)
        {
            return successVal;
        }
        std::string strBuff(_buffer);
        std::string strNumOfPackets = strBuff.substr( strBuff.find(":")+1, strBuff.find("/0")-1);
        setNumOfPackets(std::stoi(strNumOfPackets));  //convert to int.
        return successVal;
    }
    else
    {
        std::cout << "ERROR: error in read(), failed to read response." << std::endl;
        return errorVal;
    }
}

void BTReceiverTCP::sendResponseOK()
{
    static constexpr int flag = 0;
    const std::string okMessage = "200OK";
    send(_clientSocket, okMessage.c_str(), okMessage.length(), flag);
}

void BTReceiverTCP::handleTraffic()
{
    static constexpr int flag = 0;
    long double throughput;
    /* Variables for testing - C convension */
    uint64_t total_bytes_read = 0;
    uint64_t total_errors_when_reading = 0;
    uint64_t current_bytes_read = 0;

    std::cout << "Computing throughput..... " << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < _numOfPackets; ++i)
    {
        current_bytes_read = 0;
        while(_buffSize > current_bytes_read )
        {
            _valRead = recv(_clientSocket, (char *)_buffer.c_str(), (_buffSize - current_bytes_read), flag);
            if (-1 == _valRead)
            {
                ++total_errors_when_reading;
            }
            else
            {
                current_bytes_read += _valRead;
                total_bytes_read += _valRead;
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto timeInMiliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();

    throughput = calculateThroughputVal(timeInMiliSeconds);
    computeThroughput(throughput);
    std::cout << "Throughput is : " << _throughputVal << _throughputType << " per second. " << std::endl;
    sendThroughput();
}

long double BTReceiverTCP::calculateThroughputVal(long timeInMiliSeconds)
{
    long double throughputVal;
    double accurate_time = static_cast<double>(timeInMiliSeconds) / 1000.00;
    throughputVal = static_cast<double>((_numOfPackets * _buffSize)) / accurate_time;
    return throughputVal;
}

void BTReceiverTCP::sendThroughput()
{
    static constexpr int flag = 0;
    std::string strThroughputVal = std::to_string(_throughputVal);
    std::string finalThrougputMsg = strThroughputVal + " " + _throughputType;
    send(_clientSocket, finalThrougputMsg.c_str(), finalThrougputMsg.length() , flag);
}

void BTReceiverTCP::startServer(const int &maxConnectionRequests)
{
    try
    {
        createSocket();
        bindSocket();
        listenForConnection(maxConnectionRequests);
    }
    catch (const std::runtime_error &ex)
    {
        std::cout << ex.what() << std::endl;
        return;
    }

    if(receiveNumOfPackets())
    {
        sendResponseOK();
        handleTraffic();
    }
}

