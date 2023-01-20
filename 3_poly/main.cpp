#include "poly.h"
#include <iostream>

int main (void) {
    int N1 = 5, N2 = 10;
    double *p = new double [N1+1];
    double *d = new double [N2+1];

    p[0] = 1;
    p[1] = 2;
    p[4] = 50;

    d[0] = 10;
    d[1] = 20;
    d[6] = 3;

    Poly p1(N1, p);
    Poly p2(N2, d);
    std::cout << p1;
    std::cout << p2;

    p1 = p2 - p1;
    std::cout << p1;
    std::cout << p2;

    return 0;
}