//
// Created by hagai on 02/12/2020.
//

#include "BTpcppDPDK.hpp"

void BTpcppDPDK::initializeDpdk()
{
    /* Intialize DPDK device: */
    bool retVal;
    _coreMaskToUse = pcpp::getCoreMaskForAllMachineCores();
    retVal = pcpp::DpdkDeviceList::initDpdk(_coreMaskToUse, MBUF_POOL_SIZE);
    if(!retVal)
    {
        throw std::runtime_error("ERROR: error in initDpdk(), (Maybe you forgot to run init-dpdk.sh..?)");
    }
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

    _device = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(DEVICE_ID_1);
    if (_device == nullptr)
    {
        throw std::runtime_error("Cannot find _device with port: 0 ");
    }
    return successVal;
}

int BTpcppDPDK::openDpdkDevices()
{
    static constexpr int errorVal = -1;
    static constexpr int successVal = 0;
    int reVal;

    reVal = _device->openMultiQueues(_device->getTotalNumOfRxQueues(),
                                     _device->getTotalNumOfTxQueues());
    if(!reVal)
    {
        throw std::runtime_error( "ERROR: error in openDpdkDevices(), Couldn't open _device ");
    }

    reVal = _device->isOpened();
    if(!reVal)
    {
        throw std::runtime_error( "ERROR: error in openDpdkDevices(), Device is not opened. ");
    }
    return successVal;
}

void BTpcppDPDK::setWorker()
{
    appWorkerThread = std::make_unique<AppWorkerThread>(_device,
                                                        _role,
                                                        _ipAddr,
                                                        _buffSize,
                                                        _numOfPackets);

    _workers.push_back(appWorkerThread.get());
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
/* Don't need it at this time. */
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

    while(!appWorkerThread->_isFinished)
    {
        sleep(1);
    }
    appWorkerThread->stop();
}
