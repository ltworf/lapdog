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

#include <stdlib.h>
#include <stdio.h>

#include "hwaddr.h"
#include "split.h"

using namespace std;

void hwaddr::set_hwaddr(std::string& hw_addr) {
    addr = hwtoi(hw_addr);
}


void hwaddr::set_hwaddr(const char *hw_addr) {
    string s=string(hw_addr);
    addr = hwtoi(s);
}


bool hwaddr::is_equal(const char *hw_addr) {
    string s=string(hw_addr);
    return addr== hwtoi(s);
}


bool hwaddr::is_equal(const std::string hw_addr) {
    return addr == hwtoi(hw_addr);
}


/**
 * Returns a string with the usual representation of the
 * hw address AA:BB:CC:DD:EE:FF
 */
string hwaddr::strrepr() {
    char dest[18];
    dest[17]='\0';

    for (int i=2; i < 8; i++) {
        uint64_t addr_copy = addr;
        uint8_t byte = (addr_copy >> ((7-i)*8)) & 0xFF;

        snprintf(dest+((i-2)*3),3,"%.2X", byte);

        if (i<7)
            dest[((i-2)*3)+2] = ':';
    }

    return string(dest);
}

/**
 * Returns the integer representation of a HW address in
 * string format.
 **/
uint64_t hwaddr::hwtoi(const std::string& hw_addr) {
    vector<string> bytes = split(hw_addr,':',6);
    uint64_t addr =0;

    for (size_t i =0; i<bytes.size(); i++) {
        if (bytes[i].size() !=2)
            return 0;

        uint8_t byte = (uint8_t)strtol(bytes[i].c_str() ,NULL,16);
        addr <<= 8;
        addr |= byte;
    }
    return addr;
}


bool hwaddr::operator==(const hwaddr &other) {
    return addr == other.addr;
}


bool hwaddr::operator==(const uint64_t &other) {
    return addr == other;
}
