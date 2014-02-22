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

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "advsystem.h"

/**
 * Same as system(3) but detaches and changes the uid
 **/
void advsystem(int uid, int gid, const char* command) {
    if (strlen(command)==0)
        return;

    int status;
    pid_t pid = fork();
    if (pid<0) {
        syslog(LOG_ERR,"Fork failed");
        return;
    }

    if (pid == 0) {
        //Child, fork again
        setuid(uid);
        setgid(gid);
        pid = fork();
        if (pid<0) {
            syslog(LOG_ERR,"Detach fork failed");
            exit(1);
        }

        if (pid==0) {
            exit(0);
        } else {
            system(command);
            exit(0);
        }

    }
    if (pid != 0) { //Father, wait for child to detach
        waitpid(pid,&status,0);
    }
}
