/*
 * lapdog - take actions on devices (dis)appearance on the LAN
 * Copyright (C) 2014-2017  Salvo Tomaselli <tiposchi@tiscali.it>
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
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <string>

#include "configuration.h"
#include "hwaddr.h"

class device {
public:
    device(const char* conffile, const char* name);
    const hwaddr get_hw_addr();
    std::string device_name();
    void missed();
    void responded();
    void in_arp(bool);
    void dump(int fd);
    bool present();
    bool is_gateway();
private:
    std::string on_appear_action;
    std::string on_disappear_action;
    std::string name;
    int count;
    int max_misses;
    int uid;
    int gid;
    bool is_in_arp_table;
    bool gateway;
    hwaddr hw_addr;
    configuration *config;
};

#endif // DEVICE_H
