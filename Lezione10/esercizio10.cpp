#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include "mpi.h"
#include "random.h"
#include "system.h"

using namespace std;

int main(int argc, char* argv[]) {

    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 110;
    //ofstream costs("./Costi/Costi_migr" + to_string(rank) + ".txt");
    ofstream costs("./Costi/Costi_nomigr" + to_string(rank) + ".txt");
    Random rnd1;
    rnd1.setAll(rnd1, rank);

    City province[n];
    ifstream file("cap_prov_ita.dat");
    if (!file.is_open()) {
        if (rank == 0) cerr << "Errore: Impossibile aprire cap_prov_ita.dat!" << endl;
        MPI_Finalize();
        return -1;
    }
    for (int i=0; i<n; i++) {
        file >> province[i].x >> province[i].y;
    }
    file.close();

    Population genitori(province, rank);
    Population figli(province, rank);
       
    int generazione = 0;
    int N_pop = genitori.get_n();

    const int N_migr = 50;              //migrazione ogni 50 generazioni
    int send[n];
    int receive[n];

    for (int i=0; i<6000; i++) {
        genitori.ordering();
        costs << i << " " << genitori.getindividual(0).getcost() << endl;

        //MIGRAZIONE TRA CONTINENTI
        if (i>0 && i%N_migr==0 && size>1) {
        
            int selected = genitori.selection();
            for (int k=0; k<n; k++) {
                send[k] = genitori.getindividual(selected).getcity(k);             //vettore di invio dell'individuo 0 locale
            }

            MPI_Request req;
            MPI_Status status;
            MPI_Isend(send, n, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD, &req);
            MPI_Recv(receive, n, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &status);
            MPI_Wait(&req, &status);

            for (int c=0; c<n; c++) {
                genitori.getindividual(selected).setcity(c, receive[c]);          //sostituzione dell'individuo immigrato con l'individuo peggiore della popolazione
            }
            genitori.getindividual(selected).check();
            genitori.ordering();
        }

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
            if (r4 < 0.1) figli.getindividual(j) = genitori.inversion(genitori.getindividual(selected));
            figli.getindividual(j).check();
            if (r0 < 0.7) {
                int selected2 = genitori.selection();
                figli.getindividual(j) = genitori.crossover(genitori.getindividual(selected), genitori.getindividual(selected2));
                figli.getindividual(j).check();

                if (j<N_pop-1) {
                    figli.getindividual(j+1) = genitori.crossover(genitori.getindividual(selected2), genitori.getindividual(selected));
                    figli.getindividual(j+1).check();
                }
            }
        }
        swap(genitori, figli);
        generazione++;
    }
        
    genitori.ordering();
    costs.close();

    double cost_0 = genitori.getindividual(0).getcost();
    needed locale;
    needed globale;
    locale.cost = cost_0;
    locale.rank = rank;
    MPI_Allreduce(&locale, &globale, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);        //trova il rank con il costo minimo tra tutti

    if (rank == globale.rank) {
        //ofstream coord("Coordinate_migr.txt");
        ofstream coord("Coordinate_nomigr.txt");
        for (int i=0; i<n; i++) {
            int temp = genitori.getindividual(0).getcity(i);
            coord << setw(12) << province[temp].x << setw(12) << province[temp].y << endl;
        }
        cout << "Rank migliore: " << globale.rank << endl;
        coord.close();
    }

    MPI_Finalize();
    return 0;
}