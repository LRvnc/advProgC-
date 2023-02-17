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

	srand48(time(NULL));

	int client_socket, ret;
	int NUM_SERVERS = argc - 1;
	
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

	// Generating matrices
	int M, N, P;

	std::cout << "M N P: ";
    std::cin >> M >> N >> P;
    std::cout << std::endl;
    
    double** A = matrixAlloc(M, N);
	double** B = matrixAlloc(N, P);
	double** C = matrixAlloc(M, P);

	// Test
	matmul(A, B, C, M, N, P);
	printMatrix(C, M, P);

	// Sending Matrix dimensions
	write(client_socket, &N, sizeof(int));
	write(client_socket, &P, sizeof(int));

	// Sending A
	int r = M % NUM_SERVERS;
	int l1 = 0, L;
	int l2 = M / NUM_SERVERS + r;

	for (int i = 0; i < NUM_SERVERS; i++) {

		// Sending l1 -> l2 lines of A
		L = l2 - l1;
		write(client_socket, &L, sizeof(int));

		for (int j = l1; j < l2; j++) {
			write(client_socket, A[j], N*sizeof(double));
		}

		l1 = l2;
		l2 += M/NUM_SERVERS;
	}

	// Sending B (complete)
	for (int i = 0; i < N; i++) {
		write(client_socket, B[i], sizeof(double));
	}

	// Receiving C
	l1 = 0;
	l2 = M / NUM_SERVERS + r;

	for (int i = 0; i < NUM_SERVERS; i++) {

		// Receiving l1 -> l2 lines of C
		L = l2 - l1;

		for (int j = l1; j < l2; j++) {
				read(client_socket, C[j], sizeof(double));
		}

		l1 = l2;
		l2 += M/NUM_SERVERS;
	}

	// Verification
	printMatrix(C, M, P);
	
	return 0;
}
