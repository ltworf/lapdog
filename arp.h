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

#ifndef ARP_H
#define ARP_H

#include <vector>

#include "hwaddr.h"

#include "arp_table_entry.h"

#define ARP_TABLE_FILE "/proc/net/arp"

class arp {
public:
    arp();
    void rescan();
    const char* find_ip_addr(const hwaddr &hw_addr);
    void dump(int fd);
private:
    std::vector<arp_table_entry> entries;
};

#endif // ARP_H
