#include "base.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

void* getImg (void* acc_s) {

    int accept_socket = *((int *)acc_s);
    off_t filesize;
    time_t mtime;
    
    char filename[20] = "webcam_server.jpg";
    
    while (true) {
        readStat(accept_socket, filesize, mtime);
        readFile(accept_socket, filename, filesize);
    }

    return NULL;
}

void main (int argc, char** argv) {

    serverSetup(MY_PORT, getImg);

}