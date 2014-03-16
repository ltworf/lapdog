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
lapdog uses cmake to build and depends upon liboping.

```
mkdir build
cd build
cmake ..
make
su -c "make install"
```

It will install `/etc/init.d/lapdog` too.

Configuration
=============
Main configuration file is in `/etc/lapdog.conf`.
A directory called `/etc/lapdog` is used to hold the configuration files
for the various devices. See in the directory `doc` for details on the
configuration.

Signals
=======
- SIGHUP will cause a reload of the configuration
- SIGUSR1 will cause a dump of the internal state to be stored in
  `/run/lapdog.dump`
