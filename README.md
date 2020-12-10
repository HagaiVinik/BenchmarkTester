# BenchmarkTester
* Description: This software is used to measure speed of packets traffic between two endpoints.
## Features:
* TCP test
* UDP test
* DPDK TCP test //not ready yet
* DPDK UDP test //not ready yet

All tests let client decide size of _buffer and number of packets to send.

#### DPDK: 
To use DPDK features you will need to install:
* DPDK platform (can be found on DPDK website).
* PcapPlusPlus API (can be found on Github).

## Example  for executing program :
##### execute: "./BenchmarkTester -h" for more info.
* Server side: "./BenchmarkTester -s -b 1024 "
* Client side: "./BenchmarkTester -c -b 1024 -p 1024 " (p - num of packets)
* DPDK option: "-dpdk tcp" (tcp/udp) // not available

### Details:
#### Author: Hagai Vinik.
#### Date: 1/12/2020
#### Note: This software is published under the GNU free licence, version 3.