#include <iostream>
#include <cmath>
#include "random.h"
#include "funzioni.h"
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

double psi(double x, double mu, double sigma) {
    return exp(-(x-mu)*(x-mu)/(2*sigma*sigma))+exp(-(x+mu)*(x+mu)/(2*sigma*sigma));
}

double V(double x) {
    return pow(x, 4) - 5./2.*x*x;
}

double psi_minus(double x, double mu, double sigma) {
    return -exp(-pow(x-mu,2)/(2*pow(sigma,2))) + exp(-pow(x+mu,2)/(2*pow(sigma,2)));
}

//valore locale dell'energia, ovvero (H psi)/psi
double H_psi(double x, double mu, double sigma) {
    return -0.5*((x*x+mu*mu)/(pow(sigma,4)) - 1/(pow(sigma,2)) + 2*x*mu/(pow(sigma,4))/psi(x,mu,sigma)*psi_minus(x,mu,sigma)) + V(x);
}

bool metro_psi(Random& rnd, double xold, double xnew, double mu, double sigma, int& accepted){
  bool decision = false;
  double acceptance = min(1., (psi(xnew,mu,sigma)*psi(xnew,mu,sigma)/(psi(xold,mu,sigma)*psi(xold,mu,sigma))));
  if(rnd.Rannyu() < acceptance) { 
    decision = true;
    accepted++;
  }
  return decision;
}

bool metro_H(Random& rnd, double delta, double temp) {
    bool decision = false;
    double acceptance = min(1., exp(-delta/temp));
    if (rnd.Rannyu() < acceptance) decision = true;

    return decision;
}

double move(Random& rnd, double x, double delta, double mu, double sigma, int& accepted) {
    double shift = rnd.Rannyu(-1., 1.)*delta;
    double xnew = x+shift;
    if(metro_psi(rnd, x, xnew, mu, sigma, accepted)) return xnew;
    else return x;
}

double error(double av2, double av, int n) {
    if (n<=1) return 0;
    return sqrt((av2 - av*av)/(n-1));
}

void MC(Random& rnd, sistema& sist, double aveH[], double& H, double& err) {
    double sum;
    double sum_H = 0, sum2_H = 0;

    for (int i=0; i<sist.N; i++) {
        sum = 0;
        for (int j=0; j<sist.L; j++) {
            sist.x = move(rnd, sist.x, sist.delta_x, sist.mu, sist.sigma, sist.accep);
            sum += H_psi(sist.x, sist.mu, sist.sigma);
        }
        aveH[i] = sum/sist.L;
        sum_H += aveH[i];
        sum2_H += aveH[i]*aveH[i];
    }
    H = sum_H/sist.N;
    sum2_H /= sist.N;
    err = error(sum2_H, H, sist.N-1);
}