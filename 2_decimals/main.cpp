#include <iostream>
#include <cstring>

bool is_finished (int *p, int N) {

    int i;

    for (i = 0; i < N; i++){
        if (p[i] != 0) {
            return false;
        }
    }
    return true;
}

void table_sum (int *p, int *k, int N) {
    // Sum of tables *p and *k, stores in table *p, with dimension N

    int i;
    int elements_sum, value_pass = 0;

    for (i = N-1; i >= 0; i--) {
        elements_sum = p[i] + k[i] + value_pass;
        p[i] = elements_sum % 1000; // remainder is saved
        value_pass = elements_sum / 1000;
    }
}

void print_dyn_table (int *p, unsigned int N) {

    unsigned int i = 0;

    for (i = 0; i < N; i++) {
        if (p[i] < 10) {
            std::cout << "00" << p[i];
        }
        else if (p[i] < 100) {
            std::cout << "0" << p[i];
        }
        else {
            std::cout << p[i];
        }        
        std::cout << " ";
    }
    std::cout << std::endl;

}

int main (void) {

    unsigned int k=0;

    // Number of decimals    
    std::cout << "Please enter an integer value: ";
    std::cin >> k;

    // Creating table (dyn. allocation)
    int *vec1 = new int [k];
    int *vec2 = new int [k];

    memset(vec1, 0, k*sizeof(int));
    memset(vec2, 0, k*sizeof(int));

    vec1[k-1] = 551;
    vec1[k-2] = 650;
    vec1[k-3] = 1;
    print_dyn_table(vec1, k);
    vec2[k-1] = 650;
    vec2[k-2] = 750;
    vec2[k-3] = 3;
    print_dyn_table(vec2, k);

    table_sum(vec1, vec2, k);
    print_dyn_table(vec1, k);

    // main return
    return 0;
}