# BenchmarkTester
* Description: This software is used to measure speed of packets traffic between two endpoints.
## Features:
* TCP test
* UDP test
* DPDK TCP test //not ready yet
* DPDK UDP test //not ready yet

All tests let client decide size of buffer and number of packets to send.

#### DPDK: 
To use DPDK features you will need to install:
* DPDK platform (can be found on DPDK website).
* PcapPlusPlus API (can be found on Github).

## Example  for executing program :
* Server side: "./BenchmarkTester -s -buffer 1024 -local"
* Client side: "./BenchmarkTester -c -buffer 1024 -nop 1024 -local" (nop - num of packets)
* DPDK option: "-dpdk tcp" (tcp/udp)

### Details:
#### Author: Hagai Vinik.
#### Date: 1/12/2020
#### Note: This software is published under the GNU free licence, version 3.