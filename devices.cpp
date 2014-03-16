/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  Salvo Tomaselli <tiposchi@tiscali.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string>
#include <syslog.h>

#include <oping.h>

#include "devices.h"

#include <iostream>


devices::devices() {
    this->config = configuration::getconfig();

    this->load_config();

    // Initial ARP scan
    this->ping_scanner = new scanner(config->lower_range,config->higher_range);
    this->ping_scanner->scan();
    this->arp_table = new arp();
}

devices::~devices() {
    delete this->ping_scanner;
    delete this->arp_table;

    //TODO delete the devices
}

void devices::load_config() {

    devices_list.clear();

    // Load devices config
    struct dirent * dir_entry;
    syslog(LOG_INFO, "Reading config from %s", config->confdir);

    DIR *confdir = opendir(config->confdir);
    if (confdir == NULL) {
        syslog(LOG_ERR,"Unable to list configuration directory");
        exit(1);
    }

    while (dir_entry = readdir(confdir)) {
        if (dir_entry->d_name[0] == '.')
            continue;
        std::string fname = std::string(config->confdir) + std::string("/") + std::string(dir_entry->d_name);
        syslog(LOG_INFO,"Loading configuration from %s", fname.c_str());

        device dev = device(fname.c_str());
        devices_list.push_back(dev);
    }

    closedir(confdir);
}

void devices::ping_all() {
    pingobj_t *pinger;
    pinger = ping_construct();

    bool net_scan_needed = false;

    //Prepare the list of hosts to ping
    for (int i=0; i<this->devices_list.size(); i++) {
        char* ip = this->arp_table->find_ip_addr(this->devices_list[i].get_hw_addr());

        if (ip == NULL) {
            devices_list[i].missed();
            net_scan_needed = true;
            continue;
        }

        ping_host_add(pinger,ip);
    }

    //Ping the hosts
    ping_send(pinger);

    //Check the replies

    int c = 0;
    for (pingobj_iter_t *iter = ping_iterator_get (pinger);
            iter != NULL;
            iter = ping_iterator_next(iter)) {

        double latency;
        size_t latency_size = sizeof(latency);


        ping_iterator_get_info(iter,PING_INFO_LATENCY,&latency,&latency_size);

        if (latency > 0) {
            devices_list[c].responded();
        } else if (latency < 0) {
            devices_list[c].missed();
            net_scan_needed = true;
        }
        c++;
    }

    if (net_scan_needed) {
        this->ping_scanner->scan();
        this->arp_table->rescan();
    }

cleanup:
    ping_destroy(pinger);
}

void devices::dump(int fd) {
    dprintf(fd, "Configured devices\n");
    for (int i = 0; i < this->devices_list.size(); i++)
        this->devices_list[i].dump(fd);
}

