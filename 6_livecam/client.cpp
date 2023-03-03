#include "base.h"
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>  

int main (int argc, char* argv[]) {

    // Client connection
    int client_socket = clientConnection(MY_PORT, argc, argv);

    if (client_socket == -1) {
        std::cerr << "clientConnect() error" << std::endl;
        return -1;
    }

    while (true) {

        // Take pic
        system("python3 capture_image.py");

        // File metadata
        struct stat st;

        if (stat("webcam.jpg", &st) == -1) {
            std::cerr << "stat() error" << std::endl;
            return -1;
        }

        // Send image stats
        sendStat(client_socket, st);

        // Send image
        char filename[20] = "webcam.jpg";
        sendFile(client_socket, filename, st.st_size);

        // Refresh
        sleep(5);

    }

    return 0;

}