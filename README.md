lapdog
======

Take actions when specific devices appear/disappear from your LAN.

lapdog is a service that monitors the presence/absence of the devices on your
LAN and executes some actions accordingly.

It pings the devices to discover if they are connected or not. And uses their
MAC address and not their IP, so it works on networks with DHCP.

Needs to run as root to be able to send ICMP packets.


Compile
=======
Lapdog uses cmake to build.

```
mkdir build
cd build
cmake ..
make
```
