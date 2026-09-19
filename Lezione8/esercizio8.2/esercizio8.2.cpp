#include <iostream>
#include <cmath>
#include "random.h"
#include "funzioni.h"
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;


int main(){
    
    Random rnd;
    rnd.setAll(rnd);
    sistema sist;

    int M_SA = 80000;
    int N_SA = 400;
    int L_SA = M_SA/N_SA;

    sist.M = 50000;
    sist.N = 100;
    sist.L = sist.M/sist.N;
    sist.mu = 0.85;
    sist.delta_mu = 0.2;
    sist.sigma = 0.6;
    sist.delta_sigma = 0.15;
    sist.x = 0.;
    sist.delta_x = 2.8*sist.sigma;
    sist.temp = 3;
    sist.delta_temp = (sist.temp-sist.temp_f)/N_SA;
    sist.temp_f = 0.;
    sist.accep = 0;
    
    double shift_sigma, shift_mu;
    double H = 0, H_new = 0;
    double err = 0, err_new = 0;
    
    ofstream Parametri("Dati.dat");
    Parametri << "TEMP:\tMU:\tSIGMA:\tH:\tERROR:" << endl;
    
    double aveH[sist.N];
    double aveH_new[sist.N];
    MC(rnd, sist, aveH, H, err);
    
    sistema new_sist;
    for (int i=0; i<M_SA; i++) {
        new_sist = sist;
        shift_mu = rnd.Rannyu(-1.,1)*sist.delta_mu;
        shift_sigma = rnd.Rannyu(-1.,1)*sist.delta_sigma;
        
        if (new_sist.mu+shift_mu>0 && new_sist.sigma+shift_sigma>0.2) {
            new_sist.mu += shift_mu;
            new_sist.sigma += shift_sigma;
        }
        
        MC(rnd, new_sist, aveH_new, H_new, err_new);
        
        if (metro_H(rnd, H_new-H, new_sist.temp)) {
            sist = new_sist;
            H = H_new;
            err = err_new;
        }
        Parametri << sist.temp << "\t" << sist.mu << "\t" << sist.sigma << "\t" << H << "\t" << err << endl;
        
        if ((i+1)%L_SA==0) sist.temp -= sist.delta_temp;
    }
    Parametri.close();
    
    MC(rnd, sist, aveH, H, err);
    double sum = 0;
    double sum2 = 0;
    ofstream Hamilton("H.dat");
    Hamilton << "#:\t<H>:\tERROR:" << endl;
    for (int i=0; i<sist.N; i++) {
        sum += aveH[i];
        sum2 += aveH[i]*aveH[i];
        H = sum/(i+1);
        Hamilton << i << "\t" << H << "\t" << error(sum2/(i+1), H, i) << endl;
    }
    Hamilton.close();
    
    double bin_0 = -(sist.mu + 3*sist.sigma);
    double bin_size = -(2.*bin_0/100.);
    double psi_ave[100] = {0};
    double psi_ave2[100] = {0};
    double psi_err[100] = {0};
    sist.x = 0;
    sist.delta_x = 2.5*sist.sigma;
    sist.accep = 0;
    
    for (int i=0; i<sist.N; i++) {
        double psi_hist[100] = {0};
        
        for (int j=0; j<sist.L; j++) {
            sist.x = move(rnd, sist.x, sist.delta_x, sist.mu, sist.sigma, sist.accep);
            int x_hist = int((sist.x-bin_0)/bin_size);
            if (x_hist >= 0 && x_hist < 100) psi_hist[x_hist]++;
        }

        for (int j=0; j<100; j++) {
            psi_ave[j] += psi_hist[j]/sist.L;
            psi_ave2[j] += (psi_hist[j]/sist.L)*(psi_hist[j]/sist.L);
        }
    }
    ofstream Psi("Psi.dat");
    Psi << "BINS:\tAVE:\tERRORS:" << endl;
    for (int i=0; i<100; i++) {
        psi_ave[i] /= sist.N;
        psi_ave2[i] /= sist.N;
        psi_err[i] = error(psi_ave2[i], psi_ave[i], sist.N-1);
        Psi << bin_0 + (i+0.5)*bin_size << "\t" << psi_ave[i]/bin_size << "\t" << psi_err[i]/bin_size << endl;
    }

    return 0;
}