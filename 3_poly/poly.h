#pragma once
#include <iostream>

class Poly {
    /*
        N = Polynomial degree (int)
        p = Table with Polynomial Coefficients (double)
    */

    public:
        Poly(int N = -1, double *p = nullptr); // Constructor 1
        ~Poly(); // Destructor
        Poly(const Poly &other); // Copy Constructor
        void operator = (const Poly &other); // Define = operator
        Poly operator + (const Poly &p2); // Define + operator
        Poly operator - (const Poly &p2); // Define - operator
        friend std::ostream &operator << (std::ostream &stream, const Poly &p); // Print Poly class (overwrite << operator)


    private:
        int deg;
        double *coefs;

};

