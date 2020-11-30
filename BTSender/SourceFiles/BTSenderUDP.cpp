//
// Created by hagai on 24/11/2020.
//

#include "../HeaderFiles/BTSenderUDP.h"

BTSenderUDP::BTSenderUDP(const int &buffSize, const std::string &ipAddr,
                         const int &numOfPackets) :BTSender(ipAddr, buffSize, numOfPackets)
{
}

void BTSenderUDP::createSocket()
{
    const int failedVal = 0;
    if ( (this->sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        throw std::runtime_error("ERROR: error in createSocket(), socket() failed");
    }

    std::memset(&this->serverAddr, 0, sizeof(this->serverAddr));

    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(this->port);
    if(this->ipAddr == "0.0.0.0")
        this->serverAddr.sin_addr.s_addr = INADDR_ANY;
    else if(this->ipAddr == "127.0.0.1")
        this->serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    this->serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void BTSenderUDP::sendNumOfPackets()
{
    int flag = 0;
    std::string strNumOfPackets = std::to_string(this->numOfPackets);
    std::string msg = "num_of_packets:";
    msg.append(strNumOfPackets);
    std::cout << "message is: " << msg << std::endl;
    int retVal = sendto(this->sock, msg.c_str(), msg.length(),MSG_CONFIRM,
                    (const struct sockaddr *) &this->serverAddr,
                                sizeof(this->serverAddr));
}

int BTSenderUDP::receiveOkResponse()
{
    int flag = 0;
    int successVal = 0;
    int errorVal = -1;
    this->valRead = recvfrom(this->sock, (char *)this->buffer.c_str(), this->buffSize,
                 MSG_WAITALL, (struct sockaddr *) &this->serverAddr,
                 &this->serverAddrLen);

    std::string message(this->buffer.substr(0, this->valRead));
    this->buffer.resize(buffSize, 1);
    if(valRead != errorVal)
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

int BTSenderUDP::sendTraffic()
{
    std::cout << "Sending traffic....." << std::endl;
    int flag = 0;
    for(int i = 0; i < this->numOfPackets; ++i)
    {
        sendto(this->sock, this->buffer.c_str(), this->buffer.length(),
               MSG_CONFIRM, (const struct sockaddr *) &this->serverAddr,
               sizeof(this->serverAddr));
    }
    std::cout << "Finished sending traffic" << std::endl;
    return 0;
}

int BTSenderUDP::receiveThroughputResponse()
{
    int flag = 0;
    int successVal = 0;
    int errorVal = -1;
    this->valRead = recvfrom(this->sock, (char *)this->buffer.c_str(), this->buffSize,
                             MSG_WAITALL, (struct sockaddr *) &this->serverAddr,
                             &this->serverAddrLen);

    /*  NOTE: Check message on a string type   */
    std::string throughput(this->buffer);
    printResponse(throughput);
    return this->valRead;
}

void BTSenderUDP::startClient()
{
    int retVal = -1;
    try{
        createSocket();
    } catch (const std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
        return;
    }
    sendNumOfPackets();
    retVal = receiveOkResponse();
    sendTraffic();
    retVal = receiveThroughputResponse();
}
