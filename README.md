# BenchmarkTester

## `Description:` `This software is used to measure speed of packets traffic between two endpoints.`

##### Details:
* Author: `Hagai Vinik`.
* Date: `1/12/2020`

## Features:
* TCP test
* UDP test
* DPDK test (either TCP/UDP)

All tests let client decide size of buffer and number of packets to send.

#### DPDK: 
To use DPDK features you will need to install:
* DPDK platform (can be found on DPDK website).
* PcapPlusPlus API (can be found on Github).

PAY ATTENTION:
 * To run dpdk test, please initialize first the shell script:
 'setup-dpdl.sh' that could be found in the PcapPlusPlus main folder.
 * Run test with sudo privilege 

## Example  for executing program :
##### execute: "./BenchmarkTester -h" for more info.
* Server side: "./BenchmarkTester -s -b 1024 "
* Client side: "./BenchmarkTester -c -b 1024 -p 1024 " (p - num of packets)
* DPDK option: "sudo ./BenchmarkTester -d -s"
* DPDK option: "sudo ./BenchmarkTester -d -c <Server's IP address>"


##### *This software is published under the GNU free licence, version 3.*

