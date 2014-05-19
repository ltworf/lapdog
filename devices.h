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

#ifndef DEVICES_H
#define DEVICES_H

#include <vector>

#include "arp.h"
#include "configuration.h"
#include "device.h"
#include "scanner.h"


class devices {
public:
    static devices * getinstance();
    std::vector<bool> present();
    std::vector<std::string> names();
    void ping_all();
    void load_config();
    void dump(int fd);
private:
    devices();
    ~devices();
    bool has_gateway;
    void rescan();
    configuration *config;
    std::vector<device> devices_list;
    arp *arp_table;
    scanner *ping_scanner;
};

#endif // DEVICES_H
