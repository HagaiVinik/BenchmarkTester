//
// Created by hagai on 02/12/2020.
//

#include "BTpcppDPDK.hpp"


BTpcppDPDK::BTpcppDPDK(const int &buffSize, const std::string &ipAddr)
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

int BTpcppDPDK::findDpdkDevices()
{
    static constexpr int errorVal = -1;
    static constexpr int successVal = 0;

    std::cout << "trying to find DPDK device....." << std::endl;
    _device = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(DEVICE_ID_1);
    if (_device == nullptr)
    {
        std::cout << "Cannot find _device with port: " << std::endl << DEVICE_ID_1 << std::endl;
        return errorVal;
    }
    std::cout << "Done." << std::endl;
    return successVal;
}

int BTpcppDPDK::openDpdkDevices()
{
    static constexpr int errorVal = -1;
    static constexpr int successVal = 0;
    int reVal;

    std::cout << "trying to open DPDK devices....." << std::endl;

    reVal = _device->openMultiQueues(_device->getTotalNumOfRxQueues(),
                                     _device->getTotalNumOfTxQueues());
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
    return successVal;
}

void BTpcppDPDK::setWorker()
{
    std::cout << "setting Workers....." << std::endl;

    auto* appWorkerThread = new AppWorkerThread(_device,
                                                AppWorkerThread::RECEIVER,
                                                _ipAddr,
                                                _buffSize,
                                                1024);
    _workers.push_back(appWorkerThread);

    std::cout << "Done." << std::endl;
}

void BTpcppDPDK::startWorkerThreads()
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

void BTpcppDPDK::startServer()
{
    findDpdkDevices();
    openDpdkDevices();
    setFilter();
    setWorker();
}

void BTpcppDPDK::registerToEvent()
{
    pcpp::ApplicationEventHandler::getInstance().onApplicationInterrupted( onApplicationInterrupted, nullptr);
}

void BTpcppDPDK::setFilter()
{
    pcpp::PortFilter portFilter(PORT, pcpp::SRC_OR_DST);
    _device->setFilter(portFilter);
}

/**
 * onApplicationInterrupted() - callback function.
 */
void BTpcppDPDK::onApplicationInterrupted(void* cookie)
{

}

long double BTpcppDPDK::calculateThroughputVal(long timeInMiliSeconds)
{
    long double throughputVal;
    double accurate_time = static_cast<double>(timeInMiliSeconds) / 1000.00;
    throughputVal = static_cast<double>((_numOfPackets * _buffSize)) / accurate_time;
    return throughputVal;
}
