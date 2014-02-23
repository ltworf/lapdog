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
lapdog uses cmake to build.

```
mkdir build
cd build
cmake ..
make
make install
```

Signals
=======
- SIGHUP will cause a reload of the configuration
- SIGUSR1 will cause a dump of the internal state to be stored in
  `/run/lapdog.dump`
