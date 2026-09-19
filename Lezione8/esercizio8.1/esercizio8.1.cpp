#include <iostream>
#include <cmath>
#include "random.h"
#include "funzioni.h"
#include <fstream>
#include <iomanip>

using namespace std;

int main() {

    Random rnd;
    rnd.setAll(rnd);
    int M = 50000;
    int N = 100;
    int L = M/N;
    double mu = 0.85;
    double sigma = 0.6;
    double delta = 2;

    double x = 0.;
    int accepted = 0;
    double ave[N];
    ofstream Punto1("Punto1.dat");
    
    double sum_prog = 0;
    double sum2_prog = 0;

    for (int i = 0; i < N; i++) {
        double sum = 0;
    
        for (int j = 0; j < L; j++) {
            x = move(x, delta, mu, sigma, rnd, accepted);
            sum += H_psi(x, mu, sigma);
        }
        
        ave[i] = sum / double(L);
        sum_prog += ave[i];
        sum2_prog += ave[i]*ave[i];
    
        Punto1 << i+1 << setw(12) << sum_prog/double(i+1) << setw(12) << error(sum2_prog/double(i+1), sum_prog/double(i+1), i+1) << endl;
    }
    Punto1.close();
}