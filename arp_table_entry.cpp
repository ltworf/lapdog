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

#include <string.h>

#include "arp_table_entry.h"

arp_table_entry::arp_table_entry(const char* ip_addr,const char* hw_addr) {
    this->hw_addr.set_hwaddr(hw_addr);
    strncpy(this->ip_addr,ip_addr,sizeof(this->ip_addr));
}

bool arp_table_entry::has_hw_addr(const hwaddr &hw_addr) {
    return this->hw_addr == hw_addr;
}

const char* arp_table_entry::get_ip_addr() {
    return this->ip_addr;
}

const char *arp_table_entry::get_hw_addr() {
    return this->hw_addr.strrepr().c_str();
}
