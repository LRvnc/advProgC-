#include <iostream>
#include <cstring>

bool is_finished (int *p, int N)
{

    int i;

    for (i = 0; i < N; i++)
    {
        if (p[i] != 0)
        {
            return false;
        }
    }
    return true;
}

void table_sum (int *p, int *k, int N)
{
    // Sum of tables *p and *k, stores in table *p, with dimension N

    int i;
    int elements_sum, value_pass = 0;

    for (i = N-1; i >= 0; i--)
    {
        elements_sum = p[i] + k[i] + value_pass;
        p[i] = elements_sum % 1000; // remainder is saved
        value_pass = elements_sum / 1000;
    }
}

void table_divide (int *p, unsigned int l, unsigned int N)
{
    // Divide table *p by l. Stores in table *p, with dimension N

    unsigned int i;
    int elements_div, value_pass = 0;

    for (i = 0; i < N; i++)
    {
        elements_div = p[i] + value_pass*1000;
        p[i] = elements_div / l; 
        value_pass = elements_div % l; // remainder is saved
    }

}

void print_dyn_table (int *p, unsigned int N)
{

    unsigned int i = 0;

    for (i = 0; i < N; i++)
    {
        if (p[i] < 10)
        {
            std::cout << "00" << p[i];
        }
        else if (p[i] < 100)
        {
            std::cout << "0" << p[i];
        }
        else
        {
            std::cout << p[i];
        }        
        // std::cout << " ";
    }
    std::cout << std::endl;
}

int main (void) {

    unsigned int n = 0, k = 3;

    // Number of decimals    
    std::cout << "Please enter an integer value: ";
    std::cin >> n;

    n /= 3;

    // Creating table (dyn. allocation)
    int *decimals_e = new int [n];
    int *aux_table = new int [n];

    memset(decimals_e, 0, n*sizeof(int));
    memset(aux_table, 0, n*sizeof(int));

    // Init aux_table
    aux_table[0] = 500; // (1/2 = 0.5)

    while (not is_finished(aux_table, n))
    {
        table_sum(decimals_e, aux_table, n);
        table_divide(aux_table, k, n);
        k++;
    }

    print_dyn_table(decimals_e, n);
    
    // main return
    return 0;
}