/* Methods of Poly class */

#include "poly.h"
#include <string.h>
#include <iostream>
#include <algorithm>

// Constructor 1
Poly::Poly(int N, double *p) {

    deg = N;
    coefs = nullptr;

    if (deg >= 0) {
        coefs = new double [deg+1];
        memcpy(coefs, p, (deg+1)*sizeof(double));
    }

}

// Destructor
Poly::~Poly() {
    if (coefs != 0) {
        delete[]coefs;
    }
}

// Copy Constructor
Poly::Poly(const Poly &other) {
    deg = other.deg;
    coefs = nullptr;
    if (deg >= 0) {
        coefs = new double [deg+1];
        memcpy(coefs, other.coefs, (deg+1)*sizeof(double));
    }
}

// = operator
void Poly::operator = (const Poly &other) {
    if (this != &other) {

        if (coefs != nullptr) {
            delete[]coefs;
        }

        deg = other.deg;
        coefs = nullptr;
        if (deg >= 0) {
            coefs = new double [deg+1];
            memcpy(coefs, other.coefs, (deg+1)*sizeof(double));
        }
    }
}

// + operator

Poly Poly::operator + (const Poly & p2) {

    int p3_deg = std::max(deg, p2.deg);

    int i;

    double *temp1 = new double [p3_deg+1];
    double *temp2 = new double [p3_deg+1];

    memset(temp1, 0, p3_deg*sizeof(double));
    memset(temp2, 0, p3_deg*sizeof(double));

    memcpy(temp1, coefs, (deg+1)*sizeof(double));
    memcpy(temp2, p2.coefs, (p2.deg+1)*sizeof(double));

    for (i = 0; i < p3_deg+1; i++) {
        temp1[i] = temp1[i] + temp2[i];
    }

    return Poly(p3_deg, temp1);
}

// - operator

Poly Poly::operator - (const Poly & p2) {

    int p3_deg = std::max(deg, p2.deg);

    int i;

    double *temp1 = new double [p3_deg+1];
    double *temp2 = new double [p3_deg+1];

    memset(temp1, 0, p3_deg*sizeof(double));
    memset(temp2, 0, p3_deg*sizeof(double));

    memcpy(temp1, coefs, (deg+1)*sizeof(double));
    memcpy(temp2, p2.coefs, (p2.deg+1)*sizeof(double));

    for (i = 0; i < p3_deg+1; i++) {
        temp1[i] = temp1[i] - temp2[i];
    }

    return Poly(p3_deg, temp1);
}

// Print Poly class (overwrite << operator)
std::ostream &operator << (std::ostream &stream, const Poly &p) {

    int i;

    stream << "Degree: " << p.deg << std::endl;

    stream << "Coefs:  ";

    for (i = 0; i < p.deg + 1; i++) {
        stream << p.coefs[i] << " ";
    }
    std::cout << std::endl << std::endl;

    return stream;
}