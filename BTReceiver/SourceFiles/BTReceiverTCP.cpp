//
// Created by Hagai on 19/11/2020.
//

#include "../HeaderFiles/BTReceiverTCP.h"


BTReceiverTCP::BTReceiverTCP(const int &buffSize ,const std::string &ipAddr) : BTReceiver(ipAddr, buffSize)
{}

void BTReceiverTCP::createSocket()
{
    const int failedVal = 0;   //retVal of socket()

    if((this->serverFd = socket(AF_INET, SOCK_STREAM, 0)) == failedVal)
    {
        throw std::runtime_error("ERROR: Socket() failed. ");
    }
    if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &this->opt, sizeof(this->opt)))
    {
        throw std::runtime_error("ERROR:: setSockOpt() failed");
    }

    /*Note: supports binding to loopBackConnetion and anyConnection only*/
    this->address.sin_family = AF_INET;
    if(this->ipAddr == "0.0.0.0")
        this->address.sin_addr.s_addr = INADDR_ANY;
    else if(this->ipAddr == "127.0.0.1")
        this->address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    this->address.sin_port = htons( this->port);
}

void BTReceiverTCP::bindSocket()
{
    const int successVal = 0;
    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < successVal)
    {
        throw std::runtime_error("ERROR: error in bindSocket(), bind() failed. ");
    }
}

void BTReceiverTCP::listenForConnection(const int &maxConnectionRequests)
{
    const int successVal = 0;
    if (listen(this->serverFd, maxConnectionRequests) < successVal)
    {
        throw std::runtime_error("ERROR: error in listenForConnection(), listen() failed. ");
    }
    std::cout << "Socket is listening on: '" << this->ipAddr << "', port:" << this->port << std::endl;
    if ((this->clientSocket = accept(this->serverFd, (struct sockaddr *)&this->address,
                                     (socklen_t*)&this->addrLen)) < successVal)
    {
        throw std::runtime_error("ERROR: error in listenForConnection(), listen() failed. ");
    }
    std::cout << "client connected. " << std::endl;
}

int BTReceiverTCP::receiveNumOfPackets()
{
    const int flag = 0;
    int successVal = 1;
    int errorVal = -1;

    valRead = recv(clientSocket, (char *)this->buffer.c_str(), this->buffSize, flag);
    if(valRead != errorVal)
    {
        if(valRead == 0)
        {
            return successVal;
        }
        std::string strBuff(buffer);
        std::string strNumOfPackets = strBuff.substr( strBuff.find(":")+1, strBuff.find("/0")-1);
        int numOfPackets = std::stoi(strNumOfPackets);
        setNumOfPackets(numOfPackets);
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
    int flag = 0;
    std::string okMessage = "200OK";
    send(clientSocket, okMessage.c_str(), okMessage.length(), flag);
}

void BTReceiverTCP::handleTraffic()
{
    const int flag = 0;
    int i = 0;
    /* Variables for testing - C convension */
    unsigned long total_bytes_read = 0;
    unsigned long total_errors_when_reading = 0;
    unsigned long current_bytes_read = 0;

    std::cout << "Computing throughput..... " << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    for(i = 0; i < this->numOfPackets; ++i)
    {
        current_bytes_read = 0;
        while( this->buffSize > current_bytes_read )
        {
            valRead = recv(clientSocket, (char *)this->buffer.c_str(), (this->buffSize - current_bytes_read), flag);
            if (-1 == valRead)
            {
                total_errors_when_reading++;
            }
            else
            {
                current_bytes_read += valRead;
                total_bytes_read += valRead;
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto timeInMiliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
    double accurate_time = (double)timeInMiliSeconds / (double)1000.00;
    long double throughput = (double)(numOfPackets * buffSize) / (double)accurate_time;
    computeThroughput(throughput);
    std::cout << "Throughput is : " << this->throughputVal << this->throughputType <<" per second. " << std::endl;
    sendThroughput();
}

void BTReceiverTCP::sendThroughput()
{
    int flag = 0;
    std::string strThroughputVal = std::to_string(this->throughputVal);
    std::string finalThrougputMsg = strThroughputVal + " " + this->throughputType;
    send(clientSocket, finalThrougputMsg.c_str(), finalThrougputMsg.length() , flag);
}

void BTReceiverTCP::startServer(const int &maxConnectionRequests)
{
    try
    {
        createSocket();
        bindSocket();
        listenForConnection(maxConnectionRequests);
    } catch (const std::runtime_error &ex){
        std::cout << ex.what() << std::endl;
        return;
    }

    if(receiveNumOfPackets())
    {
        sendResponseOK();
        handleTraffic();
    }
}
