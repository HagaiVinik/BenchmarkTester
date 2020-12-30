//
// Created by hagai on 02/12/2020.
//

#include "AppWorkerThread.hpp"


/*AppWorkerThread::AppWorkerThread(AppWorkerConfig& appWorkerConfig) :
                                m_appWorkerConfig(appWorkerConfig),
                                _stop(true),
                                _coreId(MAX_NUM_OF_CORES+1)
{
}*/

AppWorkerThread::AppWorkerThread(pcpp::DpdkDevice* device, uint16_t role, pcpp::Packet* packetPtr,
                                 int buffSize, int numOfPackets):
        _device(device),
        _role(role),
        _packetPtr(packetPtr),
        _buffSize(buffSize),
        _numOfPackets(numOfPackets),
        _stop(true),
        _coreId(MAX_NUM_OF_CORES + 1)  //This initialization is like -1 , impossible value.
{
}

bool AppWorkerThread::run(uint32_t coreId)
{
    std::cout << "Started working Thread" <<std::endl;

    _coreId = coreId;   // Register coreId for this worker
    _stop = false;
    int packetsCounter = 0;
    pcpp::MBufRawPacket* packetArr = new pcpp::MBufRawPacket[_buffSize];

    if (_role == RECEIVER)
    {
        while (packetsCounter < _numOfPackets)
        {
            uint16_t packetsReceived = _device->receivePackets(&packetArr, _buffSize, 0);
            if (packetsReceived > 0)
            {
                packetsCounter += packetsReceived;
            }
        }
        std::cout << packetsCounter << " All packets Received!." << std::endl;

        // free packet array (frees all mbufs as well)
        for (int i = 0; i < _buffSize; i++)
        {
            if (packetArr[i] != NULL)
            {
                delete packetArr[i];
            }
        }
    }
    else if(_role == SENDER)
    {
        //Build Packet with size of m_bufSize
        while (packetsCounter < _numOfPackets)
        {
            // send it m_numOfPackets times.
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
