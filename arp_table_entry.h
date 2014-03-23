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

#ifndef ARP_TABLE_ENTRY_H
#define ARP_TABLE_ENTRY_H

#include "hwaddr.h"

class arp_table_entry {
public:
    arp_table_entry(const char* ip_addr,const char* hw_addr);
    ~arp_table_entry();
    bool has_hw_addr(const char* hw_addr);
    const char* get_ip_addr();
    const char* get_hw_addr();
private:
    char ip_addr[16];
    hwaddr *hw_addr=NULL;
};

#endif // ARP_TABLE_ENTRY_H
