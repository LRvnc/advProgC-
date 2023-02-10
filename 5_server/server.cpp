#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "base.h"

/*
struct sockaddr_in {
	short sin_family;			// Address family (AF_INET)
	unsigned short sin_port;	// Server IP Port
	struct in_addr sin_addr;	// Server IP Address
	char sin_zero[8];			// Padding for network conexion (use htons)
};
*/

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

// Matrix Multiplication (Single Thread)
void matmul (double **A, double **B, double**C, int M, int N, int P) {
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void * hconnect (void * acc_s) {
	int accept_socket = *((int *)acc_s);

	close(accept_socket);
	free(acc_s);
	pthread_detach(pthread_self());
	return NULL;
}

int main (int argc, char ** argv) {

	int server_socket = 1, bind_return = 1, listen_return = 1, accept_socket = 1;

	pthread_t tid;

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(MY_PORT);
	inet_aton("127.0.0.1", &service.sin_addr);

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Step 1: creating the server socket (returns 0 if good)
															   // AF_INET = IPv4 (address family)
															   // SOCK_STREM = TCP socket (socket type)
															   // IPPROTO_TCP = TCP protocol (protocol to be used)

	if (server_socket == -1) {
		std::cout << "Error creating server socket: " << server_socket << std::endl;
		return 0;
	}
	else {
		std::cout << "Server socket created." << std::endl;
	}

	bind_return = bind(server_socket, (struct sockaddr*)&service, sizeof(service)); // Step 2: binding the server (binding IP and Port)
																				    // server_socket = unbound socket
																				    // service = address to assign to the socket from the sockaddr struct
																				    // sizeof(service) = socket structure length in bytes

	if (bind_return == -1) {
		std::cout << "Error binding server socket: " << bind_return << std::endl;
		return 0;
	}
	else {
		std::cout << "Server binded." << std::endl;
	}

	listen_return = listen(server_socket, 10); // Step 3: listen to client requests
											   // server_socket = binded server
											   // 10 = number of max. connected clients (backlog)

	if (listen_return == -1) {
		std::cout << "Error listening requests: " << listen_return << std::endl;
		return 0;
	}
	else {
		std::cout << "Listening..." << std::endl;
	}

	while (true) {
		accept_socket = accept(server_socket, NULL, NULL); // Step 4: accepting connection
													// server_socket = listening socket
													// first NULL = optional structure containing the client address info
													// second NULL = size of this structure (if included)
	
		if (accept_socket == -1) {
			std::cout << "Error accepting request: " << accept_socket << std::endl;
			return 0;
		}
		else {
			std::cout << "Request accepted. Socket duplicated. Listening..." << std::endl;
		}

		int* acc_s = new int;
		*acc_s = accept_socket;
		pthread_create(&tid, NULL, hconnect, (void*)acc_s);

	}

	return 0;
}