#include "stdlib.h"
#include <time.h>

//#include "BTServer.h"
#include "../BTReceiver/HeaderFiles/BTReceiver.h"
#include "../BTReceiver/HeaderFiles/BTReceiverTCP.h"
#include "../BTReceiver/HeaderFiles/BTReceiverUDP.h"
#include "../BTSender/HeaderFiles/BTSender.h"
#include "../BTSender/HeaderFiles/BTSenderTCP.h"
#include "../BTSender/HeaderFiles/BTSenderUDP.h"

/**
 * main method of the application
 */

int main(int argc, char* argv[])
{
    std::string myIpAddr = "127.0.0.1";
    int buffSize = 1024;

    if(argc > 1)
    {
        BTSenderUDP *bt = new BTSenderUDP(buffSize,myIpAddr, 1024*1024*2);
        bt->startClient();

    }else{
        BTReceiverUDP* bt = new BTReceiverUDP(buffSize, myIpAddr);
        bt->startServer();
        //delete(bt);
    }
    return 0;
}
