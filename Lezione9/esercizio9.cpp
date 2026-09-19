#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "random.h"
#include "system.h"

using namespace std;

int main() {

    const int n = 34;
    //ofstream coord("Coordinate_c.txt");
    //ofstream costs("Costi_c.txt");
    ofstream coord("Coordinate_q.txt");
    ofstream costs("Costi_q.txt");
    Random rnd1;
    rnd1.setAll(rnd1);

    City cerchio[n];
    for (int i=0; i<n; i++) {
        double ang = rnd1.Rannyu(0, n);
        cerchio[i].x = cos(2*M_PI*ang/n);
        cerchio[i].y = sin(2*M_PI*ang/n);
    }
    
    City quadrato[n];
    for (int i=0; i<n; i++) {
        quadrato[i].x = rnd1.Rannyu();
        quadrato[i].y = rnd1.Rannyu();
    }

    //Population genitori(cerchio);
    //Population figli(cerchio);
    Population genitori(quadrato);
    Population figli(quadrato);
       
    int generazione = 0;
    int N_pop = genitori.get_n();

    for (int i=0; i<5000; i++) {
        genitori.ordering();

        double sum_half = 0;                                    //plot costo elemento 0 e costo medio della metà migliore della popolazione
        int n_half = genitori.get_n() / 2;
        for (int k=0; k<n_half; k++) {
            sum_half += genitori.getindividual(k).getcost();
        }
        double half = sum_half / n_half;
        costs << i << " " << genitori.getindividual(0).getcost() << " " << half << endl;

        for (int j=0; j<N_pop; j++) {
            double r0 = rnd1.Rannyu();
            double r1 = rnd1.Rannyu();
            double r2 = rnd1.Rannyu();
            double r3 = rnd1.Rannyu();
            double r4 = rnd1.Rannyu();
            int selected = genitori.selection();
            if (r1 < 0.1) figli.getindividual(j) = genitori.pair_permutation(genitori.getindividual(selected));
            if (r2 < 0.1) figli.getindividual(j) = genitori.contig_permutation(genitori.getindividual(selected));
            if (r3 < 0.1) figli.getindividual(j) = genitori.contig_contig_permutation(genitori.getindividual(selected));
            if (r4 < 0.1) figli.getindividual(j) = genitori.inversion(genitori.getindividual(selected)); figli.getindividual(j).check();
            if (r0 < 0.7) {
                int selected2 = genitori.selection();
                figli.getindividual(j) = genitori.crossover(genitori.getindividual(selected), genitori.getindividual(selected2));
                if (j<N_pop-1) {
                    figli.getindividual(j+1) = genitori.crossover(genitori.getindividual(selected2), genitori.getindividual(selected));
                    figli.getindividual(j+1).check();
                }
            }
            figli.getindividual(j).check();
        }
        swap(genitori, figli);
        generazione++;
        }
        
    genitori.ordering();
        
    for (int i=0; i<n; i++) {
        int temp = genitori.getindividual(0).getcity(i);
        //coord << setw(12) << cerchio[temp].x << setw(12) << cerchio[temp].y << endl;
        coord << setw(12) << quadrato[temp].x << setw(12) << quadrato[temp].y << endl;
    }

    coord.close();
    costs.close();
    return 0;
}