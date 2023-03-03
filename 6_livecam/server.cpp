#include "base.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

void* getImg (void* acc_s) {

    int accept_socket = *((int *)acc_s);
    off_t filesize = 0;
    time_t mtime;
    
    char filename[20] = "webcam_server.jpg";
    int flag;
    
    while (true) {

        read(accept_socket, &flag, sizeof(int));

        if (flag == 0) {
            readStat(accept_socket, filesize, mtime);
            readFile(accept_socket, filename, filesize);
        }
    }

    return NULL;
}

int main (int argc, char** argv) {

    serverSetup(MY_PORT, getImg);
    return 0;

}