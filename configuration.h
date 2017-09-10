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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CONF_FILE "/etc/lapdog.conf"
#define CONF_STATUS_DUMP_FILE "/run/lapdog.dump"

class configuration {
public:
    static configuration *getconfig();

    char* lower_range;
    char* higher_range;

    char* confdir;

    int sleep_time;

private:
    configuration();
    ~configuration();
};

#endif // CONFIGURATION_H
