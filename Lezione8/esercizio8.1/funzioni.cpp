#include <iostream>
#include <cmath>
#include "random.h"
#include <fstream>
#include <iomanip>

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

bool metro(double xold, double xnew, double mu, double sigma, Random& rnd, int& accepted){
  bool decision = false;
  double acceptance;
  acceptance = min(1., (psi(xnew,mu,sigma)*psi(xnew,mu,sigma)/(psi(xold,mu,sigma)*psi(xold,mu,sigma))));
  if(rnd.Rannyu() < acceptance) { 
    decision = true;
    accepted++;
  }
  return decision;
}

double move(double x, double delta, double mu, double sigma, Random& rnd, int& accepted) {
    double shift = rnd.Rannyu(-1., 1.)*delta;
    double xnew = x+shift;
    if(metro(x, xnew, mu, sigma, rnd, accepted)) return xnew;
    else return x;
}

double error(double av2, double av, int n) {
    if (n<=1) return 0;
    return sqrt((av2 - av*av)/(n-1));
}