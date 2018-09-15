/*
 * lapdog - take actions on devices (dis)appearance on the LAN
 * Copyright (C) 2014-2018  Salvo Tomaselli <tiposchi@tiscali.it>
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

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include "configuration.h"
#include "devices.h"
#include "main.h"
#include "config.h"

devices *devs = devices::getinstance();

unsigned int signals = 0;

void reload_signal(int _) {
    __sync_fetch_and_or(&signals, SIGL_RELOAD);
}

void dump_signal(int _) {
    __sync_fetch_and_or(&signals, SIGL_DUMP);
}

void int_signal(int _) {
    __sync_fetch_and_or(&signals, SIGL_INT);
}

void dump_status() {
    devs->dump(1);

    int fd = open(CONF_STATUS_DUMP_FILE, O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        syslog(LOG_ERR,"Can't write status on " CONF_STATUS_DUMP_FILE);
        return;
    }

    devs->dump(fd);
    close(fd);
}

/**
 * If lapdog is already running, sends the instance
 * a signal and prints the status instead.
 *
 * In this case, the program will just terminate.
 **/
void already_running() {
    int readable = access(PIDFILE, R_OK);

    if (readable != 0) {
        return;
    }

    printf("Reading status...\n");

    //Read the pid
    int fd = open(PIDFILE, O_RDONLY);
    char buffer[128];
    memset(buffer, 0, 128);
    int read_amount = read(fd, buffer, sizeof(buffer) - 1);
    if (read_amount == sizeof(buffer) - 1) {
        syslog(LOG_DEBUG,"Pidfile content too long");
        exit(1);
    }
    close(fd);

    //Convert the pid to int
    int pid = atoi(buffer);

    //Send the signal
    kill(pid, SIGUSR1);

    //Wait for the dumpfile to be created
    {
        int count = 0;
        for (count = 0; count < 10; count++) {
            if (access(CONF_STATUS_DUMP_FILE, R_OK)==0)
                break;

            usleep(100000);
        }
        if (count == 10) {
            printf("Daemon is not responding...\n");
            exit(1);
        }
    }


    //Print the dumpfile
    fd = open(CONF_STATUS_DUMP_FILE, O_RDONLY);

    while ((read_amount = read(fd, buffer, sizeof(buffer) - 1)) != 0)
        write(1, buffer, read_amount);

    close(fd);
    unlink(CONF_STATUS_DUMP_FILE);
    exit(0);
}

void create_pidfile() {
    int fd = open(PIDFILE, O_CREAT | O_WRONLY, 0444);

    if (fd == -1) {
        syslog(LOG_ERR,"Unable to create pidfile " PIDFILE);
        exit (1);
    }

    dprintf(fd, "%d", getpid());

    close(fd);
}

void destroy_pidfile() {
    unlink(PIDFILE);
}

int main(int argc, char **argv) {
    syslog(LOG_INFO, "Starting lapdog " LAPDOG_VERSION);
    already_running();

    create_pidfile();

    configuration *config = configuration::getconfig();

    signal(SIGHUP, reload_signal);
    signal(SIGUSR1, dump_signal);
    signal(SIGINT, int_signal);
    signal(SIGTERM, int_signal);

    time_t last_ping_all = 0;
    while(true) {
        sleep(config->sleep_time);

        if (time(NULL) - last_ping_all >= config->sleep_time) {
            devs->ping_all();
            last_ping_all = time(NULL);
        }

        //Check the mask for signals occurred
        unsigned int signals_occurred = __sync_fetch_and_and(&signals, 0);
        if (signals_occurred != 0) {
            if (signals_occurred & SIGL_RELOAD)
                devs->load_config();
            if (signals_occurred & SIGL_DUMP)
                dump_status();
            if (signals_occurred & SIGL_INT) {
                destroy_pidfile();
                exit(0);
            }
        }
    }

    return 0;
}
