//
// Created by hagai on 02/12/2020.
//

#include "../HeaderFiles/BTReceiverDPDK.h"

BTReceiverDPDK::BTReceiverDPDK(const std::string &ipAddr, const int &buffSize) : BTReceiver(ipAddr, buffSize)
{
    // Intialize DPDK device:
    pcpp::CoreMask coreMaskToUse = pcpp::getCoreMaskForAllMachineCores();
    pcpp::DpdkDeviceList::initDpdk(coreMaskToUse, MBUF_POOL_SIZE);
}

int BTReceiverDPDK::findDpdkDevices()
{
    const int errorVal = -1;
    _device1 = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(DEVICE_ID_1);
    if (_device1 == NULL)
    {
        std::cout << "Cannot find _device1 with port: " << std::endl << DEVICE_ID_1 << std::endl;
        return errorVal;
    }

    _device2 = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(DEVICE_ID_2);
    if (_device2 == NULL)
    {
        std::cout << "Cannot find _device1 with port: " << std::endl << DEVICE_ID_2 << std::endl;
        return errorVal;
    }
    return 0;
}

int BTReceiverDPDK::openDpdkDevices()
{
    const int errorVal = -1;
    if (!_device1->openMultiQueues(1, 1))
    {
        std::cout <<"Couldn't open _device1 " << _device1->getDeviceId() << ", PMD "
        << _device1->getPMDName().c_str() << std::endl;
        return errorVal;
    }

    if (!_device2->openMultiQueues(1, 1))
    {
        std::cout <<"Couldn't open _device1 " << _device2->getDeviceId() << ", PMD "
                  << _device2->getPMDName().c_str() << std::endl;
        return errorVal;
    }
    return 0;
}

void BTReceiverDPDK::setWorkers()
{
    workers.push_back(new L2FwdWorkerThread(_device1, _device2));
    workers.push_back(new L2FwdWorkerThread(_device2, _device1));
}

void BTReceiverDPDK::startServer()
{

}
