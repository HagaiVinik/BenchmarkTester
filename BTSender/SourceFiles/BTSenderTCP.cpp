//
// Created by hagai on 22/11/2020.
//

#include "../HeaderFiles/BTSenderTCP.h"

BTSenderTCP::BTSenderTCP(const int &buffSize, const std::string &ipAddr) : BTSender(ipAddr)
{
    this->buffSize = buffSize;
    setNumOfPackets();
}

BTSenderTCP::BTSenderTCP(const int &buffSize, const std::string &ipAddr,
                         const int &numOfPackets) : BTSender(ipAddr)
{
    this->buffSize = buffSize;
    this->numOfPackets = numOfPackets;
}

void BTSenderTCP::createSocket()
{
    const int failedVal = 0;   //retVal of socket()
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < failedVal)
    {
        throw std::runtime_error("ERROR: error in createSocket(), socket() failed. ");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(this->port);

    if(inet_pton(AF_INET, this->ipAddr.c_str(), &serv_addr.sin_addr) <= failedVal)
    {
        throw std::runtime_error("ERROR: error in createSocket(), inet_pton() failed, Invalid address");
    }
    std::cout << "Created client socket." << std::endl;
}

void BTSenderTCP::connectSocket()
{
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        throw std::runtime_error("ERROR: error in connectSocket(), connect() failed");
    }
}

void BTSenderTCP::sendNumOfPackets()
{
    int flag = 0;
    std::string strNumOfPackets = std::to_string(this->numOfPackets);
    std::string msg = "num_of_packets:";
    msg.append(strNumOfPackets);
    std::cout << msg << std::endl;
    int retVal = send(sock, msg.c_str(), msg.length(), flag);
    std::cout << "retVal is:" << retVal << std::endl;
}

int BTSenderTCP::sendTraffic()
{
    memset(buffer,1,sizeof(buffer));
    std::cout << "numOfPackets:" << this->numOfPackets << std::endl;
    std::cout << "Sending traffic....." << std::endl;
    int flag = 0;
    for(int i = 0; i < this->numOfPackets; ++i)
    {
        send(sock, buffer, strlen(buffer), flag);
    }
    std::cout << "Finished sending traffic" << std::endl;
    return 0;
}

int BTSenderTCP::receiveOkResponse()
{
    int flag = 0;
    int successVal = 0;
    int errorVal = -1;
    valRead = recv(sock, buffer, sizeof(buffer), flag);
    /*  NOTE: Check message on a string type   */
    std::string message(buffer);
    memset(buffer,1,sizeof(buffer));
    if(valRead != errorVal)
    {
        if(message == "200OK")
        {
            std::cout << "Message is: " << message << std::endl;
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

int BTSenderTCP::receiveThroughputResponse()
{
    int flag = 0;
    int successVal = 0;
    int errorVal = -1;
    valRead = recv(sock, buffer, sizeof(buffer), flag);
    /*  NOTE: Check message on a string type   */
    std::string throughput(buffer);
    printResponse(throughput);
    return valRead;
}

void BTSenderTCP::startClient()
{
    int retVal = -1;
    try{
        createSocket();
    } catch (const std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
        return;
    }
    std::cout << "Client: Created socket." << std::endl;
    try{
        connectSocket();
    } catch (const std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
        return;
    }
    std::cout << "Client: Socket connected." << std::endl;

    sendNumOfPackets();
    retVal = receiveOkResponse();
    sendTraffic();
    retVal = receiveThroughputResponse();
    std::cout << "Client Disconnected." << std::endl;
}
