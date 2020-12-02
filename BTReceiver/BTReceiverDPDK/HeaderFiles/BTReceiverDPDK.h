//
// Created by hagai on 02/12/2020.
//

#ifndef BENCHMARKTESTER_BTRECEIVERDPDK_H
#define BENCHMARKTESTER_BTRECEIVERDPDK_H


#include <vector>
#include <unistd.h>
#include <sstream>

#include "SystemUtils.h"
#include "DpdkDeviceList.h"
#include "TablePrinter.h"

#include "L2FwdWorkerThread.h"

#include "../../HeaderFiles/BTReceiver.h"

#define MBUF_POOL_SIZE 16*1024-1
#define DEVICE_ID_1 0
#define DEVICE_ID_2 1





class BTReceiverDPDK : public BTReceiver {
public:
    BTReceiverDPDK(const std::string &ipAddr, const int &buffSize);
    ~BTReceiverDPDK() = default;

private:
    pcpp::DpdkDevice* _device1;
    pcpp::DpdkDevice* _device2;
    std::vector<pcpp::DpdkWorkerThread*> workers;

    int findDpdkDevices();
    int openDpdkDevices();
    void setWorkers();
    void startServer();
};



#endif //BENCHMARKTESTER_BTRECEIVERDPDK_H
