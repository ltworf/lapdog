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
#include <queue>

using namespace std;

devices *devices::getinstance() {
    static devices* i = new devices();
    return i;
}

vector<string> devices::names() {

    vector<string> result;

    for (size_t i=0; i< devices_list.size(); i++) {
        result.push_back(devices_list[i].device_name());
    }

    return result;
}

vector< bool > devices::present() {
    vector<bool> result;

    for (size_t i=0; i< devices_list.size(); i++) {
        result.push_back(devices_list[i].present());
    }

    return result;
}



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

        device dev = device(fname.c_str(),dir_entry->d_name);
        devices_list.push_back(dev);
    }

    closedir(confdir);
}

void devices::ping_all() {
    pingobj_t *pinger;
    pinger = ping_construct();

    bool net_scan_needed = false;

    /* Used to match indexes in the devices_list with the order of the ping results.
    The order might not be the same if some devices are not in the ARP table and are
    not pinged */
    queue<size_t> corresponding_index;

    //Prepare the list of hosts to ping
    for (size_t i=0; i<this->devices_list.size(); i++) {
        const char* ip = this->arp_table->find_ip_addr(this->devices_list[i].get_hw_addr());

        if (ip == NULL) {
            //TODO Do i really want this?
            devices_list[i].missed();
            net_scan_needed = true;
            continue;
        }

        corresponding_index.push(i);
        ping_host_add(pinger,ip);
    }

    //Ping the hosts
    ping_send(pinger);

    //Check the replies

    for (pingobj_iter_t *iter = ping_iterator_get (pinger);
            iter != NULL;
            iter = ping_iterator_next(iter)) {

        double latency;
        size_t latency_size = sizeof(latency);
        int c = corresponding_index.front();
        corresponding_index.pop();


        ping_iterator_get_info(iter,PING_INFO_LATENCY,&latency,&latency_size);

        if (latency > 0) {
            devices_list[c].responded();
        } else if (latency < 0) {
            devices_list[c].missed();
            net_scan_needed = true;
        }
    }

    if (net_scan_needed) {
        this->ping_scanner->scan();
        this->arp_table->rescan();
    }

    ping_destroy(pinger);
}

void devices::dump(int fd) {
    dprintf(fd, "Configured devices\n");
    for (size_t i = 0; i < this->devices_list.size(); i++)
        this->devices_list[i].dump(fd);
}

