#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "random.h"

using namespace std;

double error(double av, double av2, int n) {     //Function for statistical uncertainty estimation
    if (n==0) {
        return 0;
    } else {
        return sqrt((av2 - pow(av, 2))/n);
    };
};

int main () {

    Random rnd;
    int M=10000;               //Total number of throws
    int N=100;                 //Number of blocks
    int L= M/N;                //Number of throws in each block, please use for M a multiple of N
    rnd.setAll(rnd);
    double ave [N];            //vettore di medie
    double ave2 [N];           //vettore di medie quadrate
    double x;
    double sum;

    for (int i=0; i<N; i++) {
        sum = 0;    
        for (int j=0; j<L; j++) {
            sum += rnd.Rannyu();
        };        

        ave[i] = sum/L;    
        ave2[i] = ave[i]*ave[i];
    };

    ofstream Punto1("Punto1.1.txt");
    double prog;
    double prog2;

    for (int i=1; i<N+1; i++) {
        prog=0;
        prog2=0;
        for (int j=0; j<i; j++) {
            prog += ave[j];
            prog2 += ave2[j];
        };
        prog = prog/i;
        prog2 = prog2/i;
        Punto1 << i << " " << prog - 1./2 << " " << error(prog, prog2, i) << endl;
    };

    Punto1.close();

//Punto 2
    double sigma[N];                //vettore di valor medi di sigma 
    double sigma2[N];               //vettore di valor medi quadrati di sigma
    
    for (int i=0; i<N; i++) {
        sum=0;
        for (int j=0; j<L; j++) {
            x = pow((rnd.Rannyu() - 1./2), 2);
            sum += x;
        };

        sigma[i] = sum/L;    
        sigma2[i] = sigma[i]*sigma[i];
    };

    ofstream Punto2("Punto1.2.txt");

    for (int i=1; i<N+1; i++) {
        prog=0;
        prog2=0;
        for (int j=0; j<i; j++) {
            prog += sigma[j];
            prog2 += sigma2[j];
        };
        prog = prog/i;
        prog2 = prog2/i;
        Punto2 << i << " " << prog - 1./12 << " " << error(prog, prog2, i) << endl;
        };

    Punto2.close();

//Punto 3
    M = 100;                            //# di intervalli
    int N_times = 10000;                //# di iterazioni 
    int n = 10000;                      //# di random numbers
    double chi2[N_times] = {0};         //vettore di risultati di chi2

    double tmp = 0;
    for (int i=0; i<N_times; i++) {
	    int interv[M] = {0};            //vettore dei sotto intervalli

	    for (int j=0; j<n; j++) {
	        tmp = rnd.Rannyu();
	        interv[int(tmp*M)] += 1;    //aumento di 1 ogni sotto intervallo
	    }

	    double sum = 0;
	    for (int k=0; k<M; k++) {
	        tmp = interv[k] - double(n)/M;
	        sum += tmp*tmp/(double(n)/M);
	    }
	    chi2[i] = sum;
    }
    
    ofstream Punto3("Punto1.3.txt");

    for (int i=0; i<N_times; ++i) Punto3 << chi2[i] << endl;

    Punto3.close();
    return 0;
}
