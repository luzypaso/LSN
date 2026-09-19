#include <iostream>
#include <cmath>
#include "random.h"
#include <fstream>
#include <iomanip>

double psi(double x, double mu, double sigma);

double V(double x);

double psi_minus(double x, double mu, double sigma);

double H_psi(double x, double mu, double sigma);

bool metro(double xold, double xnew, double mu, double sigma, Random& rnd, int& accepted);

double move(double x, double delta, double mu, double sigma, Random& rnd, int& accepted);

double error(double av2, double av, int n);

