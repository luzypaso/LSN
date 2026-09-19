
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"

using namespace std;
 
void setRandom(Random& rnd){
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   rnd.SaveSeed();
};

double error(double av, double av2, int n) {     //Function for statistical uncertainty estimation
    if (n==0) {
        return 0;
    } else {
        return sqrt((av2 - pow(av, 2))/n);
    };
};


int main() {

    Random rnd;
    setRandom(rnd);

    //Set data
    int S0 = 100;
    int K = 100;
    int T = 1;
    double r = 0.1;
    double sigma = 0.25;

    //Call and Put for "direct sampling"
    int M = 10000;
    int L = 100;
    int N = M/L;
    double aveC[N];
    double ave2C[N];
    double aveP[N];
    double ave2P[N];

    for (int i=0; i<N; i++) {
        double sumC = 0;
        double sumP = 0;
        double tempC;
        double tempP;

        for (int j=0; j<L; j++) {
            double x = S0*exp((r-sigma*sigma/2)*T + sigma*rnd.Gauss(0,1));
            tempC = exp(-r*T)*max(0., x-K);
            tempP = exp(-r*T)*max(0., K-x);
            sumC += tempC;
            sumP += tempP;
        };
        
        aveC[i] = sumC/L;
        aveP[i] = sumP/L;
        ave2C[i] = aveC[i]*aveC[i];
        ave2P[i] = aveP[i]*aveP[i];
    };
    
    ofstream dcall("DirectCall.txt");
    ofstream dput("DirectPut.txt");

    for (int i=1; i<N+1; i++) {
        double sumC = 0;
        double sum2C = 0;
        double sumP = 0;
        double sum2P = 0;

        for (int j=0; j<i; j++) {
            sumC += aveC[j];
            sum2C += ave2C[j];
            sumP += aveP[j];
            sum2P += ave2P[j];
        };

        sumC = sumC/i;
        sum2C = sum2C/i;
        sumP = sumP/i;
        sum2P = sum2P/i;
        dcall << i << " " << sumC << " " << error(sumC, sum2C, i-1) << endl;
        dput << i << " " << sumP << " " << error(sumP, sum2P, i-1) << endl;
    };
    
    dcall.close();
    dput.close();


    //Call and Put for "discretized sampling"
    double t = double(T)/100;
    int steps = 100;

    for (int i=0; i<N; i++) {
        double sumC = 0;
        double sumP = 0;
        double tempC;
        double tempP;

        for (int j=0; j<L; j++) {

            double St = S0;

            for (int n=0; n<steps; n++) {
                St = St*exp((r-sigma*sigma/2)*t + sigma*rnd.Gauss(0,1)*sqrt(t));
            };
            tempC = exp(-r*T)*max(0., St-K);
            tempP = exp(-r*T)*max(0., K-St);
            sumC += tempC;
            sumP += tempP;
        };
        
        aveC[i] = sumC/L;
        aveP[i] = sumP/L;
        ave2C[i] = aveC[i]*aveC[i];
        ave2P[i] = aveP[i]*aveP[i];
    };
    
    ofstream dcall2("DiscretizedCall.txt");
    ofstream dput2("DiscretizedPut.txt");

    for (int i=1; i<N+1; i++) {
        double sumC = 0;
        double sum2C = 0;
        double sumP = 0;
        double sum2P = 0;

        for (int j=0; j<i; j++) {
            sumC += aveC[j];
            sum2C += ave2C[j];
            sumP += aveP[j];
            sum2P += ave2P[j];
        };

        sumC = sumC/i;
        sum2C = sum2C/i;
        sumP = sumP/i;
        sum2P = sum2P/i;
        dcall2 << i << " " << sumC << " " << error(sumC, sum2C, i-1) << endl;
        dput2 << i << " " << sumP << " " << error(sumP, sum2P, i-1) << endl;
    };
    
    dcall2.close();
    dput2.close();

    return 0;
}
