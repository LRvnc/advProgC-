#include <iostream>
#include <vector>
#include <math.h>

bool is_prime (int n) {

    int max = int(sqrt(n));
    int i = 0;

    for (i = 2; i <= max + 1; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;

}

int main (void) {

    int n = 2;
    unsigned int p;

    std::vector<int> prime_numbers;

    std::cout << "Please enter an integer value: ";
    std::cin >> p;

    if (p >= 1) {
        prime_numbers.push_back(1);
    }

    if (p >= 2) {
        prime_numbers.push_back(2);
    }

    while (prime_numbers.size() < p) {
        if (is_prime(n)) {
            prime_numbers.push_back(n);
        }
        n++;
    }

    for (std::vector<int>::iterator j = prime_numbers.begin(); j != prime_numbers.end(); ++j)
    std::cout << ' ' << *j;
    std::cout << std::endl;

    return 0;
}