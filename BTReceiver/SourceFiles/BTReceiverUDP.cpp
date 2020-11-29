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
    if ((this->serverFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
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

    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int retVal = setsockopt(this->serverFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
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
    this->valRead = recvfrom(this->serverFd, (char *)this->buffer.c_str(), sizeof(this->buffer),
                             MSG_WAITALL, ( struct sockaddr *) &this->clientAddr,
                             &len);

    this->buffer.at(valRead) = 0;
    if(valRead != errorVal)
    {
        if(valRead == 0)
        {
            return errorVal;
        }
        std::string strBuff(buffer);
        std::string strNumOfPackets = strBuff.substr( strBuff.find(":")+1, strBuff.find("/0")-1);
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
    int flag = 0;
    std::string okMessage = "200OK";
    std::cout << "sending OK response. " << std::endl;
    sendto(this->serverFd, okMessage.c_str(), okMessage.length(),
           MSG_CONFIRM, (const struct sockaddr *) &this->clientAddr,
           sizeof(this->clientAddr));
    std::cout << "OK message sent. " << std::endl;
}

void BTReceiverUDP::handleTraffic()
{
    /*
     * Note: Need to fix this method, throughput is too low,
     *          could be flags that were passed to OS.
     */

    const int errorVal = -1;
    unsigned int len = sizeof(this->clientAddr);
    unsigned long total_bytes_read = 0;
    unsigned long total_errors_when_reading = 0;
    unsigned long counter_packets_arrived = 0;
    std::cout << "handling traffic" << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();
    int iterator = 0;
    int i = 0;
    for(i = 0; i < this->numOfPackets; ++i)
    {
        this->valRead = errorVal;
        this->valRead = recvfrom(this->serverFd, (char *)this->buffer.c_str(),
                                     (sizeof(this->buffer.c_str())), MSG_WAITALL,
                                     ( struct sockaddr *) &this->clientAddr, &len);

        if(this->valRead == errorVal)
        {
            ++total_errors_when_reading;
        }
        if (this->valRead != errorVal)
        {
            counter_packets_arrived++;
            total_bytes_read += this->valRead;
        }
    }
    std::cout << "counter_packets_arrived: " << counter_packets_arrived << std::endl;
    std::cout << "total_errors_when_reading: " << total_errors_when_reading << std::endl;
    std::cout << "total_bytes_read: " << total_bytes_read << std::endl;
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Finished handling packets." << std::endl;
    auto timeInMiliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
    std::cout << "timeInMiliseconds:" << timeInMiliSeconds << std::endl;
    double accurate_time = (double)timeInMiliSeconds / (double)1000.00;
    accurate_time -= 1.00;
    std::cout << "accurateTime" << accurate_time << std::endl;
    long double throughput = (this->numOfPackets * this->buffSize) / (double)accurate_time;
    computeThroughput(throughput);
    std::cout << "Throughput is : " << this->throughputVal <<
                                    this->throughputType <<" per second. " << std::endl;
    sendThroughput();
}

void BTReceiverUDP::sendThroughput()
{
    std::string strThroughputVal = std::to_string(this->throughputVal);
    std::string finalThrougputMsg = strThroughputVal + " " + this->throughputType;
    std::cout << "Final message is: " << finalThrougputMsg << std::endl;
    sendto(this->serverFd, finalThrougputMsg.c_str(), finalThrougputMsg.length(),
           MSG_CONFIRM, (const struct sockaddr *) &this->clientAddr,
           sizeof(this->clientAddr));
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