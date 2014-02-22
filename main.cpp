#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "configuration.h"
#include "devices.h"

devices devs;

void reload_config(int _) {
    devs.load_config();
}

void dump_status(int _) {
    //TODO dump on a file
    devs.dump(1);
}

int main(int argc, char **argv) {
    configuration *config = configuration::getconfig();

    signal(SIGHUP, reload_config);
    signal(SIGUSR1, dump_status);

    while(true) {
        sleep(config->sleep_time);
        devs.ping_all();
    }

    return 0;
}




