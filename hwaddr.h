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

#ifndef HWADDR_H
#define HWADDR_H

#include <string>
#include <stdint.h>

class hwaddr {
public:
    void set_hwaddr(std::string& hw_addr);
    void set_hwaddr(const char* hw_addr);
    bool operator==(const hwaddr &other);
    bool is_equal(const char* hw_addr);
    bool is_equal(const std::string hw_addr);
    bool is_set();
    std::string strrepr();
private:
    bool addr_set = false;
    uint64_t addr=0;
    uint64_t hwtoi(const std::string& hw_addr);
};

#endif // HWADDR_H
