//
// Created by hagai on 24/11/2020.
//

#include "../HeaderFiles/BTSenderUDP.h"
BTSenderUDP::BTSenderUDP(const int &buffSize, const std::string &ipAddr) :BTSender(ipAddr)
{
    this->buffSize = buffSize;
    setNumOfPackets();
}

BTSenderUDP::BTSenderUDP(const int &buffSize, const std::string &ipAddr,
                         const int &numOfPackets) : BTSender(ipAddr)
{
    this->buffSize = buffSize;
    this->numOfPackets = numOfPackets;
}

void BTSenderUDP::createSocket()
{
    const int failedVal = 0;
    if ( (this->sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        throw std::runtime_error("ERROR: error in createSocket(), socket() failed");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    if(this->ipAddr == "0.0.0.0")
        this->serverAddr.sin_addr.s_addr = INADDR_ANY;
    else if(this->ipAddr == "127.0.0.1")
        this->serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void BTSenderUDP::sendNumOfPackets()
{
    int flag = 0;
    std::string strNumOfPackets = std::to_string(this->numOfPackets);
    std::string msg = "num_of_packets:";
    msg.append(strNumOfPackets);
    std::cout << msg << std::endl;
    int retVal = sendto(this->sock, msg.c_str(), msg.length(),MSG_CONFIRM,
                    (const struct sockaddr *) &this->serverAddr,
                                sizeof(this->serverAddr));
    std::cout << "retVal is:" << retVal << std::endl;
    std::cout << "Finished sending numOfPackets"  << msg << std::endl;
}

int BTSenderUDP::receiveOkResponse()
{
    int flag = 0;
    int successVal = 0;
    int errorVal = -1;
    this->valRead = recvfrom(this->sock, (char *)buffer, sizeof(buffer),
                 MSG_WAITALL, (struct sockaddr *) &this->serverAddr,
                 &this->serverAddrLen);

    buffer[this->valRead] = '\0';
    std::string message(buffer);
    memset(buffer, 1, sizeof(buffer));
    if(valRead != errorVal)
    {
        if (message == "200OK")
        {
            std::cout << "Message is: " << message <<std::endl;
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
    memset(buffer,1,sizeof(buffer));
    std::cout << "Sending traffic....." << std::endl;
    int flag = 0;
    std::cout << "numOfPackets:" << this->numOfPackets << std::endl;
    for(int i = 0; i < this->numOfPackets; ++i)
    {
        sendto(sock, buffer, strlen(buffer),
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
    this->valRead = recvfrom(this->sock, (char *)buffer, sizeof(buffer),
                             MSG_WAITALL, (struct sockaddr *) &this->serverAddr,
                             &this->serverAddrLen);

    /*  NOTE: Check message on a string type   */
    std::string throughput(buffer);
    printResponse(throughput);
    return valRead;
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
    std::cout << "Client: Created socket." << std::endl;
    sendNumOfPackets();
    retVal = receiveOkResponse();
    sendTraffic();
    retVal = receiveThroughputResponse();
    std::cout << "Client Disconnected." << std::endl;
}