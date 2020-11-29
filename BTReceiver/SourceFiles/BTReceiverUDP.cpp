//
// Created by hagai on 24/11/2020.
//

#include "../HeaderFiles/BTReceiverUDP.h"


BTReceiverUDP::BTReceiverUDP(const int &buffSize, const std::string &ipAddr) : BTReceiver(ipAddr, buffSize)
{
}

void BTReceiverUDP::createSocket()
{
    const int failedVal = 0;
    if ((this->serverFd = socket(AF_INET, SOCK_DGRAM, 0)) < failedVal )
    {
        throw std::runtime_error("ERROR: error in Socket(), creation error");
    }
    memset(&this->serverAddr, 0, sizeof(this->serverAddr));
    memset(&this->clientAddr, 0, sizeof(this->clientAddr));
    this->serverAddr.sin_family = AF_INET;
    if(this->ipAddr == "0.0.0.0")
        this->serverAddr.sin_addr.s_addr = INADDR_ANY;
    else if(this->ipAddr == "127.0.0.1")
        this->serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    this->serverAddr.sin_port = htons(this->port);
}

void BTReceiverUDP::setTimeOut()
{
    // setting timeout for recvfrom()
    // consider changing to SO_RCVBUFF
    timeval tv;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int retVal = setsockopt(this->serverFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if(retVal < 0)
    {
        throw std::runtime_error("ERROR: error in setTomeOut(), setsockopt() failed.");
    }
}

void BTReceiverUDP::bindSocket()
{
    if ( bind(this->serverFd, (const struct sockaddr *)&this->serverAddr,
              sizeof(this->serverAddr)) < 0 )
    {
        throw std::runtime_error("ERROR: error in bindSocket(), bind() failed");
    }
    std::cout << "Socket is Binding on: '" << this->ipAddr << "', port:" << this->port << std::endl;
}

int BTReceiverUDP::receiveNumOfPackets()
{;
    int successVal = 0;
    int errorVal = -1;
    unsigned int len = sizeof(this->clientAddr);
    this->valRead = recvfrom(this->serverFd, (char *)this->buffer.c_str(), this->buffSize,
                             MSG_WAITALL, ( struct sockaddr *) &this->clientAddr,
                             &len);

    this->buffer.at(this->valRead) = 0;
    if(valRead != errorVal)
    {
        std::string strBuff(buffer);
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
    std::string okMessage = "200OK";
    sendto(this->serverFd, okMessage.c_str(), okMessage.length(),
           MSG_CONFIRM, (const struct sockaddr *) &this->clientAddr,
           sizeof(this->clientAddr));
}

void BTReceiverUDP::handleTraffic()
{
    /*
     * Note: Need to fix this method, throughput is too low,
     *          could be flags that were passed to OS.
     */

    const int errorVal = -1;

    unsigned int len = sizeof(this->clientAddr);

    double successInPercents = 0.0;
    int numOfPacketsLost = 0;
    unsigned long numOfPacketsArrived = 0;

    std::cout << "computing throughput....." << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < this->numOfPackets; ++i)
    {
        this->valRead = errorVal;
        this->valRead = recvfrom(this->serverFd, (char *)this->buffer.c_str(),
                                     (sizeof(this->buffer.c_str())), MSG_WAITALL,
                                     ( struct sockaddr *) &this->clientAddr, &len);

        if(this->valRead == errorVal)
        {
            break;
        }
        if (this->valRead != errorVal)
        {
            numOfPacketsArrived++;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto timeInMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    double accurate_time = (double)timeInMilliSeconds / (double)1000.00;
    if(numOfPacketsArrived < this->numOfPackets)
        accurate_time -= 1.00;      // timeout set t0 1sec, if all packets arrived,
                                    //                  there is no need to calculate timeout.


    long double throughput = (this->numOfPackets * this->buffSize) / (double)accurate_time;
    computeThroughput(throughput);

    successInPercents = ((double)numOfPacketsArrived / (double)this->numOfPackets) * 100.0;
    numOfPacketsLost = this->numOfPackets - numOfPacketsArrived;
    sendThroughput(successInPercents, numOfPacketsLost);
}

void BTReceiverUDP::sendThroughput(const double &successInPercents, const int &numOfPacketsLost)
{
    std::string strThroughputVal = std::to_string(this->throughputVal);
    std::string strSuccessInPercents = std::to_string(successInPercents);
    std::string strNumOfPacketsLost = std::to_string(numOfPacketsLost);
    std::string finalThroughputMsg = strThroughputVal + this->throughputType +
                                     "(" + strSuccessInPercents + "%, lost: " +
                                     strNumOfPacketsLost + ")";

    std::cout << "Throughput is : " << finalThroughputMsg << " per second. " << std::endl;
    sendto(this->serverFd, finalThroughputMsg.c_str(), finalThroughputMsg.length(),
           MSG_CONFIRM, (const struct sockaddr *) &this->clientAddr,
           sizeof(this->clientAddr));
    std::cout << "Sent to client." << std::endl;
}

void BTReceiverUDP::startServer(const int &maxConnectionRequests)
{
    const int failedVal = -1;
    int retVal = -2;
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



/*
 * while(iterator < this->numOfPackets)
    {
        this->valRead = errorVal;
        //while(valRead == -1)
        //{
            this->valRead = recvfrom(this->serverFd, (char *)this->buffer.c_str(),
                                     (sizeof(this->buffer.c_str())), MSG_WAITALL,
                                     ( struct sockaddr *) &this->clientAddr, &len);

            /*if (this->valRead != errorVal)
            {
                counter_packets_arrived++;
                total_bytes_read += this->valRead;
                total_errors_when_reading = 0;
            }
            if(this->valRead == errorVal)
            {
                ++total_errors_when_reading;
            }
            if(total_errors_when_reading > 100000 && total_bytes_read > 0 )
            {
                this->valRead = 0;
                iterator = this->numOfPackets + 1;
            }

        }
++iterator;
}
*/