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

#include <stdio.h>
#include <string.h>

#include "arp.h"

/**
 * Automatically does rescan
 **/
arp::arp() {
    this->rescan();
}

/**
 * Returns a string containing the corresponding IP address of the device
 * according to the kernel's ARP table.
 *
 * NULL if the entry is not found.
 *
 * This function only checks inside a copy of the table. run rescan() to
 * re-load the table from the kernel.
 **/
const char* arp::find_ip_addr(const hwaddr &hw_addr) {
    for (size_t i=0; i<entries.size(); i++) {
        if (entries[i].has_hw_addr(hw_addr))
            return entries[i].get_ip_addr();
    }
    return NULL;
}

/**
 * Performs a rescan of the ARP table from the kernel, cleaning up
 * the internally stored one.
 *
 * The code is kind of copied from arp.c utility in net-tools
 **/
void arp::rescan() {
    char ip[100];
    char hwa[100];
    char mask[100];
    char line[200];
    char dev[100];
    int type, flags;
    int num;
    char empty[] = "00:00:00:00:00:00";

    entries.clear();

    FILE *file = fopen(ARP_TABLE_FILE, "r");

    //Skip first header line
    if (fgets(line, sizeof(line), file)==NULL)
        goto cleanup;

    for (; fgets(line, sizeof(line), file);) {
        num = sscanf(line, "%s 0x%x 0x%x %100s %100s %100s\n",
                     ip, &type, &flags, hwa, mask, dev
                    );
        if (num < 4)
            continue;
        if (strncmp(hwa, empty, sizeof(empty)) == 0)
            continue;

        arp_table_entry new_entry = arp_table_entry(ip,hwa);
        entries.push_back(new_entry);
    }

cleanup:
    fclose(file);
}

/**
 * Dumps on fd a printout of the current internal copy
 * of the ARP table.
 **/
void arp::dump(int fd) {
    dprintf(fd, "ARP table\n");
    for (size_t i=0; i < this->entries.size(); i++)
        dprintf(fd, "%s\t%s\n", this->entries[i].get_ip_addr(),this->entries[i].get_hw_addr());
}
