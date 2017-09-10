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

#ifndef SCANNER_H
#define SCANNER_H

#include <inttypes.h>

#include <oping.h>

class scanner {
public:
    scanner(char* lower, char* higher);
    ~scanner();
    void scan();
private:
    uint32_t range_lower;
    uint32_t range_higher;
    pingobj_t *pinger;
    void init_pinger();
};

#endif // SCANNER_H
