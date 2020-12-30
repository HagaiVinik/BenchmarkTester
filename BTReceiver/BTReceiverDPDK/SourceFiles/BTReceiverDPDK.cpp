//
// Created by hagai on 02/12/2020.
//

#include "BTReceiverDPDK.hpp"


BTReceiverDPDK::BTReceiverDPDK(const int &buffSize, const std::string &ipAddr) : BTReceiver(ipAddr, buffSize) , _device(_device)
{
    /* Intialize DPDK device: */
    bool retVal;
    std::cout << "Initializing Dpdk....." << std::endl;

    _coreMaskToUse = pcpp::getCoreMaskForAllMachineCores();
    std::cout << "_coreMaskToUse: " << _coreMaskToUse << std::endl;
    retVal = pcpp::DpdkDeviceList::initDpdk(_coreMaskToUse, MBUF_POOL_SIZE);
    if(!retVal)
    {
        std::cout << "ERROR: error in initDpdk(), failed initializing DPDK." << std::endl;
    }

    std::cout << "Done." << std::endl;
}

int BTReceiverDPDK::findDpdkDevices()
{
    std::cout << "trying to find DPDK device....." << std::endl;
    const int errorVal = -1;
    _device = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(DEVICE_ID_1);
    if (_device == nullptr)
    {
        std::cout << "Cannot find _device with port: " << std::endl << DEVICE_ID_1 << std::endl;
        return errorVal;
    }
    std::cout << "Device:" << _device->getDeviceName() << " ID:" << _device->getDeviceId() <<
              " PMD:" << _device->getPMDName() << std::endl;  // " " << _device->
    std::cout << "Done." << std::endl;
}

int BTReceiverDPDK::openDpdkDevices()
{
    const int errorVal = -1;
    int reVal;

    std::cout << "trying to open DPDK devices....." << std::endl;

    reVal = _device->openMultiQueues(_device->getTotalNumOfRxQueues(),
                                     _device->getTotalNumOfTxQueues());
    std::cout << "DeviceQueues: " << _device->getTotalNumOfTxQueues() << std::endl;
    std::cout << "DeviceQueues: " << _device->getTotalNumOfRxQueues() << std::endl;
    if(!reVal)
    {
        std::cout << "Couldn't open _device " << _device->getDeviceId() << ", PMD "
                  << _device->getPMDName().c_str() << std::endl;
        return errorVal;
    }

    reVal = _device->isOpened();
    if(!reVal)
    {
        std::cout <<"ERROR: Device is not open! please open device correctly." << std::endl;
    }
    std::cout << "Done." << std::endl;
}

void BTReceiverDPDK::setWorker()
{
    std::cout << "setting Workers....." << std::endl;

    AppWorkerThread* appWorkerThread = new AppWorkerThread(_device, 0 , 1024, 1024);
    _workers.push_back(appWorkerThread);

    std::cout << "Done." << std::endl;
}

void BTReceiverDPDK::startWorkerThreads()
{
    static constexpr int flag = 0;
    long double throughput;
    bool retVal;

    retVal = pcpp::DpdkDeviceList::getInstance().startDpdkWorkerThreads(2, _workers);
    if(!retVal)
    {
        std::cout << "ERROR handleTraffic(): error in startDpdkWorkerThreads(), couldn't start DPDK workerThreads." << std::endl;
    }
}

void BTReceiverDPDK::startServer()
{
    findDpdkDevices();
    openDpdkDevices();
    setFilter();
    setWorker();
}

void BTReceiverDPDK::registerToEvent()
{
    pcpp::ApplicationEventHandler::getInstance().onApplicationInterrupted( onApplicationInterrupted, nullptr);
}

void BTReceiverDPDK::setFilter()
{
    pcpp::PortFilter portFilter(PORT, pcpp::SRC_OR_DST);
    _device->setFilter(portFilter);
}

/**
 * onApplicationInterrupted() - callback function.
 */
void BTReceiverDPDK::onApplicationInterrupted(void* cookie)
{

}

void BTReceiverDPDK::craftPacket()
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

std::string BTReceiverDPDK::getLocalIPAddress()
{
    /** Need to figure out if this really needed, and if it is, how to get it. */
}

