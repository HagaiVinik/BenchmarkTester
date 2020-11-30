#include <SystemUtils.h>
#include <getopt.h>


#include "BTReceiver/HeaderFiles/BTReceiver.h"
#include "BTReceiver/HeaderFiles/BTReceiverTCP.h"
#include "BTReceiver/HeaderFiles/BTReceiverUDP.h"
#include "BTSender/HeaderFiles/BTSender.h"
#include "BTSender/HeaderFiles/BTSenderTCP.h"
#include "BTSender/HeaderFiles/BTSenderUDP.h"


const std::string SOFTWARE_VERSION = "1.0";


static struct option BTOptions[] =
        {
                {"client", no_argument, 0, 'c'},
                {"server", no_argument, 0, 's'},
                {"UDP", no_argument, 0, 'u'},
                {"help", no_argument, 0, 'h'},
                {"version", no_argument, 0, 'v'},
                {"numOfPackets", required_argument, 0, 'p'},
                {"buffSize", required_argument, 0, 'b'},
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
           "    -b buffSize     : sets size of the buffer\n"
           "    -p numOfPackets : sets number of packets to measure\n"
           "    -u UDP          : use UDP protocol instead of TCP\n"
           "    -d dpdk         : Use DPDK technology\n"
           "    -v version      : Display the current version and exit\n"
           "    -h help         : Display this help message and exit\n\n";
}


int main(int argc, char* argv[])
{
    /* Default values. */
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

