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


const std::string SOFTWARE_VERSION = "2.0";


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
           "BenchmarkTester [-h] [-v] [-s server-side] [-c client-side] [-i ip Address] \n"
           "                 [-u UDP] [-d dpdk ] \n"
           "\nOptions:\n\n"
           "    -c client        : starts program as a client side\n"
           "    -s server        : starts program as a server side\n"
           "    -i IP            : IP address of the specified target\n"
           "    -l Loopback      : local IP address of the machine\n"
           "    -b _buffSize     : sets size of the _buffer\n"
           "    -p _numOfPackets : sets number of packets to measure\n"
           "    -u UDP           : use UDP protocol instead of TCP\n"
           "    -d dpdk          : Use DPDK technology\n"
           "    -v version       : Display the current version and exit\n"
           "    -h help          : Display this help message and exit\n\n";
}


int main(int argc, char* argv[])
{
    /* Default values. */
    std::string BTIpAddr = "0.0.0.0";
    int buffSize = 1024;
    int numOfPackets = 1024;
    std::string BTInstance = "";
    std::string BTType = "TCP";
    std::string filter = "";
    bool useDpdk = false;


    int optionIndex = 0;
    char opt = 0;
    while((opt = getopt_long(argc, argv, "csuhvli:p:b:", BTOptions, &optionIndex)) != -1)
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
            case 'l':
                BTIpAddr = "127.0.0.1";
                break;
            case 'i':
                BTIpAddr = optarg;
                break;
            case 'd':
                useDpdk = true;
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

    if(!useDpdk)
    {
        if(BTInstance == "server")
        {
            if (BTType == "UDP")
            {
                BTReceiverUDP bt(buffSize, BTIpAddr);
                bt.startServer();
            }
            else
            {
                BTReceiverTCP bt(buffSize, BTIpAddr);
                bt.startServer();
            }
        }
        else if(BTInstance == "client")
        {
            if(BTType == "UDP")
            {
                BTSenderUDP bt(buffSize, BTIpAddr, numOfPackets);
                bt.startClient();
            }
            else
            {
                BTSenderTCP bt(buffSize, BTIpAddr, numOfPackets);
                bt.startClient();
            }
        }
    }
    else
    {
        if(BTInstance == "server")
        {
            BTpcppDPDK bt(buffSize,BTIpAddr, BTpcppDPDK::RECEIVER);
            bt.startServer();
        }
        else if(BTInstance == "client")
        {
            BTpcppDPDK bt(buffSize,BTIpAddr, BTpcppDPDK::TRANSMITTER);
            bt.startServer();
        }
    }

    return 0;
}

