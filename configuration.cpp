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
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <syslog.h>

#include "configuration.h"

configuration::configuration() {

    lower_range = "10.0.0.1";
    higher_range = "10.0.0.220";
    confdir = "/etc/lapdog";
    sleep_time = 10;

    std::string line;
    std::filebuf fb;
    if (not fb.open(CONF_FILE, std::ios::in)) {
        return;
    }
    syslog(LOG_INFO, "Loading configuration from " CONF_FILE);
    std::istream is(&fb);

    while (std::getline(is,line)) {
        std::istringstream lines(line);

        if (line[0] == '#')
            continue;
        else if (line.size() == 0)
            continue;

        std::string key;
        std::string value;

        std::getline(lines,key,'=');
        std::getline(lines,value,'=');

        if (key=="confdir") {
            confdir = new char [value.length()+1];
            std::strcpy (confdir, value.c_str());
        } else if (key=="lower_range") {
            confdir = new char [value.length()+1];
            std::strcpy (confdir, value.c_str());
        } else if (key=="higher_range") {
            confdir = new char [value.length()+1];
            std::strcpy (confdir, value.c_str());
        } else if (key=="sleep_time") {
            sleep_time = strtol(value.c_str(),NULL,10);
        }
    }

}

configuration::~configuration() {
    delete confdir;
    delete lower_range;
    delete higher_range;
}

configuration *configuration::getconfig() {
    static configuration *c = new configuration();
    return c;
}
