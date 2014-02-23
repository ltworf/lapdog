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

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>

#include "configuration.h"
#include "devices.h"
#include "main.h"

devices devs;

volatile unsigned int signals = 0;

void reload_signal(int _) {
    signals = signals | SIGL_RELOAD;
}

void dump_signal(int _) {
    signals |= SIGL_DUMP;
}

void dump_status() {
    devs.dump(1);

    int fd = open(CONF_STATUS_DUMP_FILE, O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        syslog(LOG_ERR,"Can't write status on " CONF_STATUS_DUMP_FILE);
        return;
    }

    devs.dump(fd);
    close(fd);
}

int main(int argc, char **argv) {
    configuration *config = configuration::getconfig();

    signal(SIGHUP, reload_signal);
    signal(SIGUSR1, dump_signal);

    while(true) {
        sleep(config->sleep_time);

        //Check the mask for signals occurred
        if (signals != 0) {
            if (signals & SIGL_RELOAD)
                devs.load_config();
            if (signals & SIGL_DUMP)
                dump_status();
            signals = 0;
        } else
            devs.ping_all();
    }

    return 0;
}
