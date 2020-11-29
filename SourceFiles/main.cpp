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
        BTReceiverTCP* bt = new BTReceiverTCP(buffSize, myIpAddr);
        bt->startServer();

    }else{
        BTSenderTCP *bt = new BTSenderTCP(buffSize,myIpAddr, 1024*1024);
        bt->startClient();
        //delete(bt);
    }
    return 0;
}



/*BTSenderTCP* bt = new BTSenderTCP(buffSize, myIpAddr);
    bt->createSocket();
    try
    {
        bt->connectSocket();
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    */




/*      TAL SUGGESTION:
 *  1. Class Factory: handle() - according for packet type.
 * */


/*
 * arg1 == server
 *      protocol: tcp/udp
 *      port: my own application (7500)
 *
 *      server init, start, capture, handle, computLatency
 * arg1 == client
 *      init ,connect , analyze
 *
 */
