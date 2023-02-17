#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "base.h"
#include <typeinfo>

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

struct thread_data {
	double **A, **B, **C;
	int N, P, l1, l2;
	char* ip;
};

void* request (void* args) {

	// Referencing args
	double **A, **B, **C;
	int N, P, l1, l2;
	char* ip;

	struct thread_data *arg_data;
	arg_data = (struct thread_data *) args;

	A = arg_data->A;
	B = arg_data->B;
	C = arg_data->C;
	N = arg_data->N;
	P = arg_data->P;
	l1 = arg_data->l1;
	l2 = arg_data->l2;
	ip = arg_data->ip;

	// Connecting to the server
	int client_socket, ret;

	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 	// Step 1: creating the server socket (returns 0 if good)
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

	struct sockaddr_in service;
	struct hostent * server;

	service.sin_family = AF_INET;
	service.sin_port = htons(MY_PORT);
	server = gethostbyname(ip);

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

	// Sending Matrix dimensions
	write(client_socket, &N, sizeof(int));
	write(client_socket, &P, sizeof(int));

	// Sending L lines of A
	int L = l2 - l1;
	write(client_socket, &L, sizeof(int));

	for (int j = l1; j < l2; j++) {
		write(client_socket, A[j], N*sizeof(double));
	}

	// Sending B
	for (int i = 0; i < N; i++) {
		write(client_socket, B[i], P*sizeof(double));
	}

	// Receiving C
	for (int j = l1; j < l2; j++) {
			read(client_socket, C[j], P*sizeof(double));
	}

	pthread_detach(pthread_self());
	return NULL;
}

int main (int argc, char* argv[]) {

	srand48(time(NULL));

	int NUM_SERVERS = argc - 1;
	
	// Command line ./client execution
	if (argc == 1) {
		std::cerr << "Usage: " << argv[0] 
			  << " [ IP address / server name ]" << std::endl;
		return 0;
	}

	if (argc > NUM_THREADS+1) {
		std::cerr << "You can connect up to 4 servers max" << std::endl;
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

/*
	// Test (local)
	matmul(A, B, C, M, N, P);
	printMatrix(C, M, P);

	C = matrixAlloc(M, P); // reseting C
	printMatrix(C, M, P);

*/

	// Connecting to servers
	int l1 = 0;
	int l2 = M / NUM_SERVERS;
	int counter = M % NUM_SERVERS;

	struct thread_data thread_data_array[NUM_SERVERS];
	pthread_t threads[NUM_SERVERS];

	for (int t=0; t < NUM_SERVERS; t++) {

		if (counter > 0) {
			l2++;
		}

		thread_data_array[t].A = A;
		thread_data_array[t].B = B;
		thread_data_array[t].C = C;
		thread_data_array[t].N = N;
		thread_data_array[t].P = P;
		thread_data_array[t].l1 = l1;
		thread_data_array[t].l2 = l2;
		thread_data_array[t].ip = argv[t+1];

		int rc = pthread_create(&threads[t], NULL, request, (void*) &thread_data_array[t]);

		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}

		l1 = l2;
		l2 += (M / NUM_SERVERS);
		counter--;
	}

	for (int t=0; t < NUM_SERVERS; t++) {
		pthread_join(threads[t], NULL);
	}

	printMatrix(C, M, P); // Verification

	return 0;
}
