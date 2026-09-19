
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"

using namespace std;

double error(double av, double av2, int n) {     //Function for statistical uncertainty estimation
    if (n==0) {
        return 0;
    } else {
        return sqrt((av2 - av*av)/n);    //L'INCERTEZZA ESCE NEGATIVA - DA SISTEMARE (E TOGLIERE IL -1)!!
    };
};


int main () {

    Random rnd;
    rnd.setAll(rnd);
    int M = 10000;
    int L = 100;
    int N = M/L;
    double sum;
    double sum2;
    double ave[N];
    double ave2[N];
    
    for (int i=0; i<N; i++) {
        sum=0;
        double temp=0;

        for (int j=0; j<L; j++) {
            temp = (cos(rnd.Rannyu() * M_PI/2))*M_PI/2;
            sum += temp;
        };

        ave[i] = sum/L;
        ave2[i] = ave[i]*ave[i];
    };

    ofstream Punto1("Punto1.txt");

    for (int i=1; i<N+1; i++) {
        sum = 0;
        sum2 = 0;

        for (int j=0; j<i; j++) {
            sum += ave[j];
            sum2 += ave2[j];
        };

        sum = sum/i;
        sum2 = sum2/i;
        Punto1 << i << " " << sum << " " << error(sum, sum2, i-1) << endl;
    };

    Punto1.close();


    //Punto2
    for (int i=0; i<N; i++) {
        sum=0;
        sum2=0;
        double temp=0;

        for (int j=0; j<L; j++) {
            double x = 1 - sqrt(1-rnd.Rannyu()); 
            temp = (cos(x * M_PI/2))*M_PI/2 / (2*(1-x));
            sum += temp;
        };

        ave[i] = sum/L;
        ave2[i] = ave[i]*ave[i];
    };

    ofstream Punto2("Punto2.txt");

    for (int i=1; i<N+1; i++) {
        sum = 0;
        sum2 = 0;

        for (int j=0; j<i; j++) {
            sum += ave[j];
            sum2 += ave2[j];
        };

        sum = sum/i;
        sum2 = sum2/i;
        Punto2 << i << " " << sum << " " << error(sum, sum2, i) << endl;
    };

    Punto2.close();
    return 0;
}

