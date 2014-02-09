/*
 * lapdog - take actions on devices (dis)appearance on the LAN
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>

#include <oping.h>

#include "scanner.h"

/**
 * Creates a scanner object.
 *
 * The purpose of this is to ping all the addresses in a
 * given range so that the ARP table will be updated with
 * the entries of all the hosts present in that range.
 *
 * Two parameters are the starting IP address and the ending
 * address.
 **/
scanner::scanner(char *lower, char *higher) {

    struct in_addr l;
    struct in_addr h;

    if (inet_aton(lower,&l)==0 || inet_aton(higher,&h)==0) {
        syslog(LOG_ERR,"Invalid address in scan range");
        exit(1);
    }

    this->range_lower = ntohl(l.s_addr);
    this->range_higher = ntohl(h.s_addr);

    this->init_pinger();
}

scanner::~scanner() {
    ping_destroy(this->pinger);
}

void scanner::init_pinger() {
    this->pinger = ping_construct();

    struct in_addr in;
    char *ip;

    for (uint32_t address = this->range_lower; address<= this->range_higher; address++) {
        in.s_addr = htonl(address);

        ip = inet_ntoa(in);
        if (ping_host_add(this->pinger,ip)!=0) {
            syslog(LOG_ERR,"%s",ping_get_error(this->pinger));
            exit(3);
        }
    }
}

void scanner::scan() {
    ping_send(this->pinger);
}
