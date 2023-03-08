#include "base.h"
#include <iostream>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include "stdc++.h"

void* getImg (void* acc_s) {

    int accept_socket = *((int *)acc_s);
    off_t filesize;
    time_t mtime;

    char hn[HOST_NAME_MAX] = "none", img_ext[5] = ".jpg";
    if (read(accept_socket, &(hn), HOST_NAME_MAX) == -1) {
        std::cerr << "Read hostname error" << std::endl;
    }
    std::strcat(hn, img_ext);
    
    int flag;
    
    while (true) {

        if (read(accept_socket, &flag, sizeof(int)) == -1) {
            std::cerr << "Read flag error" << std::endl;
        }

        if (flag == 0) {
            readStat(accept_socket, filesize, mtime);
            readFile(accept_socket, hn, filesize);
        }
    }

    return NULL;
}

int main (int argc, char** argv) {

    serverSetup(MY_PORT, getImg);
    return 0;

}