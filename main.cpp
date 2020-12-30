#include <getopt.h>
#include <cstdio>

#include <SystemUtils.h>

#include "BTpcppDPDK.hpp"
#include "AppWorkerThread.hpp"
#include "BTReceiver.hpp"
#include "BTReceiverTCP.hpp"
#include "BTReceiverUDP.hpp"
#include "BTSender.hpp"
#include "BTSenderTCP.hpp"
#include "BTSenderUDP.hpp"


const std::string SOFTWARE_VERSION = "1.0";


static struct option BTOptions[] =
        {
                {"client", no_argument, 0, 'c'},
                {"server", no_argument, 0, 's'},
                {"UDP", no_argument, 0, 'u'},
                {"help", no_argument, 0, 'h'},
                {"version", no_argument, 0, 'v'},
                {"_numOfPackets", required_argument, 0, 'p'},
                {"_buffSize", required_argument, 0, 'b'},
                {0, 0, 0, 0}
        };

void printVersion()
{
    std::cout << "Version " << SOFTWARE_VERSION << std::endl;
}

void printUsage()
{
    std::cout <<"\nUsage:\n"
           "-------\n"
           "copyright ©2020 BenchmarkTester, all rights reserved.\n"
           "BenchmarkTester [-h] [-v] [-o output_file] [-c packet_count] [-i filter] [-s]\n"
           "\nOptions:\n\n"
           "    -c client       : starts program as a client side\n"
           "    -s server       : starts program as a server side\n"
           "    -b _buffSize     : sets size of the _buffer\n"
           "    -p _numOfPackets : sets number of packets to measure\n"
           "    -u UDP          : use UDP protocol instead of TCP\n"
           "    -d dpdk         : Use DPDK technology\n"
           "    -v version      : Display the current version and exit\n"
           "    -h help         : Display this help message and exit\n\n";
}

/*
int main(int argc, char* argv[])
{
    /* Default values.
    std::string myIpAddr = "127.0.0.1";
    int buffSize = 1024;
    int numOfPackets = 1024;
    std::string BTInstance = "";
    std::string BTType = "TCP";
    std::string filter = "";


    int optionIndex = 0;
    char opt = 0;
    while((opt = getopt_long(argc, argv, "csuhvp:b:", BTOptions, &optionIndex)) != -1)
    {
        switch (opt)
        {
            case 0:
                break;
            case 'p':
                numOfPackets = std::stoi(optarg);
                break;
            case 'b':
                buffSize = std::stoi(optarg);
                break;
            case 'c':
                if(BTInstance == "")
                    BTInstance = "client";
                else
                {
                    std::cout << "Error: program accept only -s or -c, not both." << std::endl;
                    return -1;
                }
                break;
            case 's':
                if(BTInstance == "")
                    BTInstance = "server";
                else
                {
                    std::cout << "Error: program accept only -s or -c, not both." << std::endl;
                    return -1;
                }
                break;
            case 'u':
                BTType = "UDP";
                break;
            case 'v':
                printVersion();
                return 0;
            case 'h':
                printUsage();
                return 0;

            default:
                printUsage();
                return -1;
        }
    }
    if(BTInstance == "server")
    {
        if (BTType == "UDP")
        {
            BTReceiverUDP bt(buffSize, myIpAddr);
            bt.startServer();
        }
        else
        {
            BTReceiverTCP bt(buffSize, myIpAddr);
            bt.startServer();
        }
    }
    else if(BTInstance == "client")
    {
        if(BTType == "UDP")
        {
            BTSenderUDP bt(buffSize, myIpAddr, numOfPackets);
            bt.startClient();
        }
        else
        {
            BTSenderTCP bt(buffSize, myIpAddr, numOfPackets);
            bt.startClient();
        }
    }

    return 0;
}
*/

/*
int main(int argc, char* argv[])
{
    std::string myIpAddr = "127.0.0.1";
    int buffSize = 1024;
    BTpcppDPDK bt(buffSize, myIpAddr);
    bt.findDpdkDevices();
    bt.openDpdkDevices();
    bt.setWorker();
    bt.startWorkerThreads();
    //bt.startServer();
    //BTpcppDPDK *bt = new BTpcppDPDK(buffSize, myIpAddr);
    return 0;
}
*/

int main(int argc, char* argv[])
{
    bool retVal;
    FILE* filePtr;

    std::vector<pcpp::DpdkWorkerThread*> _workers;
    pcpp::DpdkDevice* _device;
    pcpp::CoreMask _coreMaskToUse;

    std::cout << "Main Version" << std::endl;

    _coreMaskToUse = pcpp::getCoreMaskForAllMachineCores();
    std::cout << "_coreMaskToUse: " << _coreMaskToUse << std::endl;
    retVal = pcpp::DpdkDeviceList::initDpdk(_coreMaskToUse, 4095, 0);
    if(!retVal)
    {
        std::cout << "ERROR: error in initDpdk(), failed initializing DPDK." << std::endl;
        return -1;
    }
    std::cout << "Done." << std::endl;
    //pcpp::DpdkDeviceList::getInstance().writeDpdkLogToFile(filePtr);
    _device = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(0);
    if (_device == nullptr)
    {
        std::cout << "Cannot find _device with port: " << std::endl << 0 << std::endl;
        return -1;
    }
    /*
    _device->getNumOfOpenedTxQueues();
    _device->getNumOfOpenedRxQueues();
    */
    retVal = _device->openMultiQueues(1,1);
    if(!retVal)
    {
        std::cout <<"Couldn't open _device " << _device->getDeviceId() << ", PMD "
                  << _device->getPMDName().c_str() << std::endl;
        return -1;
    }

    retVal = _device->isOpened();
    if(!retVal)
    {
        std::cout <<"ERROR: Device is not open! please open device correctly." << std::endl;
    }
    AppWorkerThread* appWorkerThread = new AppWorkerThread(_device,1, "127.0.0.1");
    AppWorkerThread* appWorkerThread2 = new AppWorkerThread(_device,2, "127.0.0.1");
    _workers.push_back(appWorkerThread);
    _workers.push_back(appWorkerThread2);
    retVal = pcpp::DpdkDeviceList::getInstance().startDpdkWorkerThreads(7, _workers);
    if(!retVal)
    {
        std::cout << "ERROR handleTraffic(): error in startDpdkWorkerThreads(), couldn't start DPDK workerThreads." << std::endl;
        return -1;
    }
    return 0;

}
