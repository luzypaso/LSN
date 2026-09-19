#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "random.h"
#include "system.h"

using namespace std;

int main() {

    const int n = 110;

    ofstream coord("Coordinate_simple.txt");
    ofstream costs("./Costi/Costi_simple.txt");
    Random rnd1;
    rnd1.setAll(rnd1);
    
    City province[n];
    ifstream file("cap_prov_ita.dat");
    for (int i=0; i<n; i++) {
        file >> province[i].x >> province[i].y;
    }

    file.close();

    Population genitori(province);
    Population figli(province);
       
    int generazione = 0;
    int N_pop = genitori.get_n();

    for (int i=0; i<6000; i++) {
        genitori.ordering();

        costs << i << " " << genitori.getindividual(0).getcost() << endl; 

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
                    j++;                                    //incremento per non sovrascrivere il secondo figlio al prossimo ciclo
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
        coord << setw(12) << province[temp].x << setw(12) << province[temp].y << endl;
    }
    coord.close();
    costs.close();
    return 0;
}