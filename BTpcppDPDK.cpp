//
// Created by hagai on 02/12/2020.
//

#include "BTpcppDPDK.hpp"

void BTpcppDPDK::initializeDpdk()
{
    /* Intialize DPDK device: */
    bool retVal;
    std::cout << "Initializing Dpdk....." << std::endl;

    _coreMaskToUse = pcpp::getCoreMaskForAllMachineCores();
    std::cout << "_coreMaskToUse: " << _coreMaskToUse << std::endl;
    retVal = pcpp::DpdkDeviceList::initDpdk(_coreMaskToUse, MBUF_POOL_SIZE);
    if(!retVal)
    {
        throw std::runtime_error("ERROR: error in initDpdk(), failed initializing DPDK.");
    }

    std::cout << "Done." << std::endl;
}

BTpcppDPDK::BTpcppDPDK(int buffSize, int numOfPackets, const std::string &ipAddr, const int role) :
                        _buffSize(buffSize),
                        _numOfPackets(numOfPackets),
                        _ipAddr(ipAddr),
                        _role(role)
{}

int BTpcppDPDK::findDpdkDevices()
{
    static constexpr int errorVal = -1;
    static constexpr int successVal = 0;

    std::cout << "trying to find DPDK device....." << std::endl;
    _device = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(DEVICE_ID_1);
    if (_device == nullptr)
    {
        throw std::runtime_error("Cannot find _device with port: 0 ");
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
        throw std::runtime_error( "ERROR: error in openDpdkDevices(), Couldn't open _device ");
    }

    reVal = _device->isOpened();
    if(!reVal)
    {
        throw std::runtime_error( "ERROR: error in openDpdkDevices(), Device is not opened. ");
    }
    std::cout << "Done." << std::endl;
    return successVal;
}

void BTpcppDPDK::setWorker()
{
    std::cout << "setting Workers....." << std::endl;

    appWorkerThread = new AppWorkerThread(_device,
                                                _role,
                                                _ipAddr,
                                                _buffSize,
                                                _numOfPackets);
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
        throw std::runtime_error("ERROR handleTraffic(): error in startDpdkWorkerThreads(), couldn't start DPDK workerThreads.");
    }
}

void BTpcppDPDK::registerToEvent()
{
    pcpp::ApplicationEventHandler::getInstance().onApplicationInterrupted( onApplicationInterrupted, nullptr);
}

/**
 * onApplicationInterrupted() - callback function.
 */
void BTpcppDPDK::onApplicationInterrupted(void* cookie)
{

}

void BTpcppDPDK::startServer()
{
    try
    {
        initializeDpdk();
        findDpdkDevices();
        openDpdkDevices();
    }
    catch (const std::runtime_error &ex)
    {
        std::cout << ex.what() << std::endl;
        return;
    }

    setWorker();    //setting arguments for worker thread.

    /* More likely to fail: */
    try
    {
        startWorkerThreads();
    }
    catch (const std::runtime_error &ex)
    {
        std::cout <<  ex.what() << std::endl;
        return;
    }

    while(!appWorkerThread->_isFinished);
    appWorkerThread->stop();
}
