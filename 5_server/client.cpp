#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "base.h"

// Matrix Dynamic Allocation
double** matrixAlloc(int M, int N=1) {

    double **p = new double *[M];

    for (int i = 0; i < M; i++) {
        p[i] = new double [N];
        memset(p[i], 0, N*sizeof(double));

        for (int j = 0; j < N; j++) {
            p[i][j] = 2*drand48()-1;
        }
    }
    return p;
}

// Matrix Dynamic Deallocation
void matrixFree (double **p, int M) {

    for (int i = 0; i < M; i++) {
        delete []p[i];
    }
    delete []p;
}

// Thread Arguments Structure
struct thread_data {
    double **A, **B, **C;
    int N, P, l1, l2;
};

// Display Matrix
void printMatrix (double **p, int M, int N) {

    int m, n;

    for (m = 0; m < M; m++) {
        for (n = 0; n < N; n++) {
            std::cout << p[m][n] << " ";
        }
    std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main (int argc, char* argv[]) {

	int client_socket, ret;

	struct sockaddr_in service;
	struct hostent * server;
	
	// Command line ./client execution
	if (argc == 1) {
		std::cerr << "Usage: " << argv[0] 
			  << " [ IP address / server name ]" << std::endl;
		return 0;
	}

	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Step 1: creating the server socket (returns 0 if good)
								   							   // AF_INET = IPv4 (address family)
								   							   // SOCK_STREM = TCP socket (socket type)
								   							   // IPPROTO_TCP = TCP protocol (protocol to be used)

	if (client_socket == -1) {
		std::cerr << "Error creating client socket: " << client_socket << std::endl;
		return 0;
	}
	else {
		std::cout << "Client socket created." << std::endl;
	}

	service.sin_family = AF_INET;
	service.sin_port = htons(MY_PORT);
	server = gethostbyname(argv[1]);

	if (server == NULL) {
		std::cerr << "gethostbyname() failed" << std::endl;
		close(client_socket);
		return 0;
	}
	else {
		memcpy(&service.sin_addr, server->h_addr, 4);
	}

	ret = connect(client_socket, (struct sockaddr*)&service, sizeof(service)); // Step 2: connecting to server
										   									   // client_socket = descriptor of client socket
										   									   // &service = structure containing the server IP address
										   									   // sizeof(service) = its corresponding size

	if (ret == -1) {
		std::cerr << "Cannot connect to remote server." << std::endl;
		close(client_socket);
		return 0;
	}

	int M, N;

	std::cout << "M N P: ";
    std::cin >> M >> N;
    std::cout << std::endl;
    
    double **A = matrixAlloc(M, N);

	write(client_socket, &A, sizeof(A));
	
	return 0;
}