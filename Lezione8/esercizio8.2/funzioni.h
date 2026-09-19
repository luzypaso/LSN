#include <iostream>
#include <cmath>
#include "random.h"
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

struct sistema {
    int M, N, L;
    double x, delta_x;
    double mu, delta_mu;
    double sigma, delta_sigma;
    double temp, delta_temp, temp_f;
    int accep;
};

double psi(double x, double mu, double sigma);

double V(double x);

double psi_minus(double x, double mu, double sigma);

//valore locale dell'energia, ovvero (H psi)/psi
double H_psi(double x, double mu, double sigma);

bool metro_psi(Random& rnd, double xold, double xnew, double mu, double sigma, int& accepted);

bool metro_H(Random& rnd, double delta, double temp);

double move(Random& rnd, double x, double delta, double mu, double sigma, int& accepted);

double error(double av2, double av, int n);

void MC(Random& rnd, sistema& sist, double aveH[], double& H, double& err);