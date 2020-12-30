//
// Created by hagai on 02/12/2020.
//

#include "AppWorkerThread.hpp"


AppWorkerThread::AppWorkerThread(pcpp::DpdkDevice* device,
                                 uint16_t role,
                                 int buffSize,
                                 int numOfPackets):
        _device(device),
        _role(role),
        _buffSize(buffSize),
        _numOfPackets(numOfPackets),
        _stop(true),
        _coreId(MAX_NUM_OF_CORES + 1)  //This initialization is like -1 , impossible value.

{
}

bool AppWorkerThread::run(uint32_t coreId)
{
    bool retVal;
    _coreId = coreId;   // Register coreId for this worker
    _stop = false;
    int packetsCounter = 0;
    auto packetArr = new pcpp::MBufRawPacket[_buffSize];

    if (_role == RECEIVER)
    {
        std::cout << "Recieving Packets....." << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();
        while (packetsCounter < _numOfPackets)
        {
            uint16_t packetsReceived = _device->receivePackets(&packetArr, _buffSize, 0);
            if (packetsReceived > 0)
            {
                packetsCounter += packetsReceived;
            }
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << packetsCounter << " All packets Received!." << std::endl;

        long timeInMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        //_throughput = calculateThroughputVal(timeInMilliSeconds);
        //computeThroughput(throughput);


        // free packet array (frees all mbufs as well)
        for (int i = 0; i < _buffSize; i++)
        {
            if (&packetArr[i] != nullptr)
            {
                delete &packetArr[i];
            }
        }
    }
    else if(_role == SENDER)
    {
        /** send packets:  */
        while (packetsCounter < _numOfPackets)
        {
            _device->sendPacket(*_packetPtr->getRawPacket());
        }
    }
    return true;
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
    pcpp::IPv4Address sourceIPAddress("10.0.0.2");
    pcpp::IPv4Address destIPAddress(_ipAddr);
    pcpp::MacAddress sourceMACAddress("ff:ff:ff:ff:ff:ff");
    pcpp::MacAddress destMACAddress("ff:ff:ff:ff:ff:ff");

    /** create a packet with capacity of 100 bytes (will grow automatically if needed) */
    pcpp::Packet newPacket(_buffSize);

    /** create layers and set arguments. */
    pcpp::EthLayer ethLayer(sourceMACAddress, destMACAddress);
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
