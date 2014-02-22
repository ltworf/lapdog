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

#ifndef DEVICE_H
#define DEVICE_H

#include <string>

#include "configuration.h"

class device {
public:
    device(const char* conffile);
    char * get_hw_addr();
    void missed();
    void responded();
    void dump(int fd);
private:
    std::string on_appear_action;
    std::string on_disappear_action;
    int count;
    int max_misses;
    char hw_addr[18];
    configuration *config;
};

#endif // DEVICE_H
