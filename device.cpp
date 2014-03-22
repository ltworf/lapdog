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

#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <syslog.h>

#include "advsystem.h"
#include "device.h"
#include "split.h"

using namespace std;

/**
 * Creates a new device object starting
 * from a configuration file.
 **/
device::device(const char* conffile, const char* name) {
    this->name = string(name);
    this->count=0;
    this->max_misses = 10;
    this->hw_addr[0] = 0;
    this->uid = 0;
    this->gid = 0;

    this->config = configuration::getconfig();


    this->count = 0;

    std::filebuf fb;
    if (not fb.open(conffile,std::ios::in)) {
        //TODO failed
    }
    std::istream is(&fb);

    vector<string> lines = split(is,'\n',0);

    for (int i=0; i<lines.size(); i++) {
        std::string line = lines[i];

        if (line[0] == '#')
            continue;
        else if (line.size() == 0)
            continue;

        vector<string> key_value = split(line,'=',2);

        string key = key_value[0];
        string value = key_value[1];

        if (key=="hw") {
            strncpy(this->hw_addr, value.c_str(), sizeof(this->hw_addr));
        } else if (key=="on_appear_action") {
            this->on_appear_action = value;
        } else if (key=="on_disappear_action") {
            this->on_disappear_action= value;
        } else if (key=="initial_count") {
            this->count = strtol(value.c_str(),NULL,10);
        } else if (key=="max_misses") {
            this->max_misses = strtol(value.c_str(),NULL,10);
        } else if (key=="uid") {
            this->uid = strtol(value.c_str(),NULL,10);
        } else if (key=="gid") {
            this->gid = strtol(value.c_str(),NULL,10);
        } else if (key=="user") {
            struct passwd * p = getpwnam(value.c_str());
            this->uid = p->pw_uid;
            this->gid = p->pw_gid;
        }
    }

    //check that hw_addr was set
    if (this->hw_addr[0] == 0) {
        syslog(LOG_ERR,"hw key missing");
        exit(1);
    }
}

/**
 * Return the hw_address associated
 **/
char *device::get_hw_addr() {
    return this->hw_addr;
}

/**
 * The device just missed a ping
 **/
void device::missed() {
    if (this->count == this->max_misses) {
        syslog(LOG_INFO, "%s disappeared, running action", hw_addr);
        advsystem(uid, gid, on_disappear_action.c_str());
        this->count ++;
    } else if (this->count < this->max_misses) {
        this->count ++;
    }
}

/**
 * The device responded to a ping
 **/
void device::responded() {
    if (this->count == this->max_misses+1) {
        syslog(LOG_INFO, "%s appeared, running action", hw_addr);
        advsystem(uid, gid, on_appear_action.c_str());
    }
    this->count = 0;
}

/**
 * Prints the status on the given file descriptor
 **/
void device::dump(int fd) {
    dprintf(fd, "%s (%s)\tmissed: %d\tmax misses: %d\n", this->name.c_str(), this->hw_addr, this->count,this->max_misses);
}
