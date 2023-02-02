// Matrix Multiplication & Threads

#include <string.h>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <pthread.h> // Threads lib
#include <chrono>
using namespace std::chrono;

#define NUM_THREADS 4

// Matrix Dynamic Allocation
double ** matrixAlloc(int M, int N=1) {

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

// Thread Arguments Structure
struct thread_data {
    double **A, **B, **C;
    int N, P, l1, l2;
};

void* matmul_part (void* args) {

    double **A, **B, **C;
    int N, P, l1, l2;

    struct thread_data *arg_data;
    arg_data = (struct thread_data *) args;

    A = arg_data->A;
    B = arg_data->B;
    C = arg_data->C;
    N = arg_data->N;
    P = arg_data->P;
    l1 = arg_data->l1;
    l2 = arg_data->l2;

    for (int i = l1; i < l2; i++) {
        for (int j = 0; j < P; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL;
}

int main (void) {

    int M, N, P, rc;
    int l1, l2, counter;

    struct thread_data thread_data_array[NUM_THREADS];

    pthread_t threads[NUM_THREADS];

    std::cout << "M N P: ";
    std::cin >> M >> N >> P;
    std::cout << std::endl;
    
    double **A = matrixAlloc(M, N);
    double **B = matrixAlloc(N, P);
    double **C = matrixAlloc(M, P);

    auto start1 = high_resolution_clock::now();
    matmul(A, B, C, M, N, P);
    auto stop1 = high_resolution_clock::now();
    
    //printMatrix(C, M, P);

    auto start2 = high_resolution_clock::now();
    l1 = 0;
    l2 = M / NUM_THREADS;
    counter = M % NUM_THREADS;

    for (int t=0; t < NUM_THREADS; t++) {

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

        rc = pthread_create(&threads[t], NULL, matmul_part, (void*) &thread_data_array[t]);

        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }

        l1 = l2;
        l2 += (M / NUM_THREADS);
        counter--;
    }

    for (int t=0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    auto stop2 = high_resolution_clock::now();

    // printMatrix(C, M, P);

    auto duration1 = duration_cast<microseconds>(stop1 - start1);
    auto duration2 = duration_cast<microseconds>(stop2 - start2);

    std::cout << "Single Thread: " << duration1.count() << " microseconds" << std::endl;
    std::cout << "Multi Thread: " << duration2.count() << " microseconds" << std::endl;

    std::cout << "Multi Thread " << duration1.count() / duration2.count() << "x faster" << std::endl;
    std::cout << std::endl;

    return 0;
}