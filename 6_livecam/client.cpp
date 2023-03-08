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

    // Send client hostname
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    if (write(client_socket, &(hostname), HOST_NAME_MAX) == -1) {
        std::cerr << "Write hostname error" << std::endl;
    }

    int flag = -1;
    time_t start = time(NULL);
    time_t now = time(NULL);

    while (true) {

        while ((now - start < 1)) {
            flag = -1;
            if (write(client_socket, &flag, sizeof(int)) == -1) {
                std::cerr << "Write flag -1 error" << std::endl;
            }
            now = time(NULL);
        }

        flag = 0;
        if (write(client_socket, &flag, sizeof(int)) == -1) {
            std::cerr << "Write flag 0 error" << std::endl;
        }

        // Take pic
        int rc = system("python3 capture_image.py");
        std::cout << "system rc: " << rc << std::endl;

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
        start = time(NULL);

    }

    return 0;

}