//
// Created by hagai on 22/11/2020.
//

#include "../HeaderFiles/BTSenderTCP.h"


BTSenderTCP::BTSenderTCP(const int &buffSize, const std::string &ipAddr,
                         const int &numOfPackets) : BTSender(ipAddr, buffSize, numOfPackets)
{
}

void BTSenderTCP::createSocket()
{
    const int failedVal = 0;   //retVal of socket()
    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < failedVal)
    {
        throw std::runtime_error("ERROR: error in createSocket(), socket() failed. ");
    }

    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(this->port);

    if(inet_pton(AF_INET, this->ipAddr.c_str(), &this->serverAddr.sin_addr) <= failedVal)
    {
        throw std::runtime_error("ERROR: error in createSocket(), inet_pton() failed, Invalid address");
    }
}

void BTSenderTCP::connectSocket()
{
    if (connect(this->sock, (struct sockaddr *)&this->serverAddr, sizeof(this->serverAddr)) < 0)
    {
        throw std::runtime_error("ERROR: error in connectSocket(), connect() failed");
    }
    std::cout << "Connected to server." << std::endl;
}

void BTSenderTCP::sendNumOfPackets()
{
    int flag = 0;
    std::string strNumOfPackets = std::to_string(this->numOfPackets);
    std::string msg = "num_of_packets:";
    msg.append(strNumOfPackets);
    int retVal = send(this->sock, msg.c_str(), msg.length(), flag);
    if (retVal == -1)
        throw std::runtime_error("ERROR: error in sendNumOfPackets(), send() failed.");
}

int BTSenderTCP::receiveOkResponse()
{
    int flag = 0;
    int successVal = 0;
    int errorVal = -1;
    this->valRead = recv(this->sock, (char *)this->buffer.c_str(), this->buffer.length(), flag);
    std::string message(this->buffer.substr(0, this->valRead));
    buffer.resize(this->buffSize,1);
    if(valRead != errorVal)
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
    int flag = 0;
    for(int i = 0; i < this->numOfPackets; ++i)
    {
        send(sock, this->buffer.c_str(),this->buffer.length(), flag);
    }
    std::cout << "Finished sending traffic. " << std::endl;
    return 0;
}

int BTSenderTCP::receiveThroughputResponse()
{
    int flag = 0;
    valRead = recv(this->sock, (char *)this->buffer.c_str(), this->buffer.length(), flag);
    std::string throughput(buffer);
    printResponse(throughput);
    return valRead;
}

void BTSenderTCP::startClient()
{
    int retVal = -1;
    try{
        createSocket();
        connectSocket();
        sendNumOfPackets();
    } catch (const std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
        return;
    }

    retVal = receiveOkResponse();
    sendTraffic();
    retVal = receiveThroughputResponse();
}
