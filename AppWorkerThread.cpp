//
// Created by hagai on 02/12/2020.
//

#include "AppWorkerThread.hpp"


AppWorkerThread::AppWorkerThread(pcpp::DpdkDevice* device,
                                 uint16_t role,
                                 const std::string &ipAddr,
                                 int buffSize,
                                 int numOfPackets):
        _device(device),
        _role(role),
        _ipAddr(ipAddr),
        _buffSize(buffSize),
        _numOfPackets(numOfPackets),
        _isFinished(false),
        _coreId(MAX_NUM_OF_CORES + 1),  // This initialization is like -1 , impossible value.
        _throughputVal(-1),                 // Default value for throughput.
        _throughputType(""),
        _stop(false)

{
    craftPacket();
}

void fillArr(pcpp::MBufRawPacket (&packetArr)[1024], pcpp::MBufRawPacket &mBufRawPacket)
{

}

bool AppWorkerThread::run(uint32_t coreId)
{
    _coreId = coreId;   // Register coreId for this worker
    int packetsCounter = 0;
    //pcpp::MBufRawPacket* packetArr[_buffSize];
    auto packetArr = std::make_unique<pcpp::MBufRawPacket*[]>(_buffSize);
    //std::unique_ptr<pcpp::MBufRawPacket> str(new pcpp::MBufRawPacket[_buffSize]);
    pcpp::MBufRawPacket mBufRawPacket;
    mBufRawPacket.initFromRawPacket(_packetPtr->getRawPacket(), _device);

    if (_role == RECEIVER)
    {
        std::cout << "waiting for Packets to arrive....." << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();
        while (packetsCounter < _numOfPackets)
        {
            uint16_t packetsReceived = _device->receivePackets(packetArr.get(), _buffSize, 0);
            if (packetsCounter == 0 && packetsReceived > 0)
            {
                startTime = std::chrono::high_resolution_clock::now();
            }
            if (packetsReceived > 0)
            {
                packetsCounter += packetsReceived;
            }
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << packetsCounter << " All packets Received!." << std::endl;

        long timeInMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        computeThroughput(timeInMilliSeconds);


        // free packet array (frees all mbufs as well)
        std::cout << "Throughput is: " << _throughputVal << _throughputType << " per second." << std::endl;
    }
    else if(_role == TRANSMITTER)
    {
        std::cout << "Reaching here" << std::endl;
        packetArr[0] = &mBufRawPacket;
        /** send packets:  */
        while (packetsCounter < _numOfPackets)
        {
            std::cout <<"Sending packet: " << packetsCounter << std::endl;
            _device->sendPacket(mBufRawPacket);
            ++packetsCounter;
        }
    }

    _isFinished = true;
    while (!_stop)
    {
        return true;
    }
    return false;
}

void AppWorkerThread::stop()
{
    _stop = true;
}

uint32_t AppWorkerThread::getCoreId() const
{
    return _coreId;
}

void AppWorkerThread::craftPacket()
{
    static constexpr auto IP_ID = 2000;
    static constexpr auto TIME_TO_LIVE = 64;
    uint8_t payloadData = 1;
    pcpp::IPv4Address sourceIPAddress("10.0.0.2");      //need to make it generic.
    pcpp::IPv4Address destIPAddress(_ipAddr);

    /** create a packet with capacity of 100 bytes (will grow automatically if needed) */
    pcpp::Packet newPacket(_buffSize);

    /** create layers and set arguments. */
    pcpp::EthLayer ethLayer(pcpp::MacAddress("ff:ff:ff:ff:ff:ff"), pcpp::MacAddress("ff:ff:ff:ff:ff:ff"));
    pcpp::IPv4Layer iPv4Layer(sourceIPAddress, destIPAddress);
    pcpp::TcpLayer newTcpLayer(PORT, PORT);
    pcpp::PayloadLayer payloadLayer(&payloadData, _buffSize, true);

    /** set IP layer specifically */
    iPv4Layer.getIPv4Header()->ipId = htons(IP_ID);
    iPv4Layer.getIPv4Header()->timeToLive = TIME_TO_LIVE;

    /** add all the layers we created */
    newPacket.addLayer(&ethLayer);
    newPacket.addLayer(&iPv4Layer);
    newPacket.addLayer(&newTcpLayer);

    /** compute all calculated fields */
    newPacket.computeCalculateFields();

    /** set packet pointer member class */
    _packetPtr = &newPacket;
}

void AppWorkerThread::computeThroughput(long timeInMiliSeconds)
{
    long double throughput;
    double accurate_time = static_cast<double>(timeInMiliSeconds) / 1000.00;
    throughput = static_cast<double>((_numOfPackets * _buffSize)) / accurate_time;

    double result = 0.0;
    if (throughput >= GB_SIZE)
    {
        result = throughput / GB_SIZE;
        _throughputType = "GB";
    }
    else if (throughput >= MB_SIZE)
    {
        result = throughput / MB_SIZE;
        _throughputType = "MB";
    }
    else if(throughput >= KB_SIZE)
    {
        result = throughput / KB_SIZE;
        _throughputType = "KB";
    }
    _throughputVal = result;
}
