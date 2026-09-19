#include <iostream>
#include <fstream>
#include <cmath>
#include "random.h"

using namespace std;

struct pos {
    int posizione[3];      //array della posizione caso discreto con tre assi 
};

struct pos_c {
    double posizione_c[3];  //array della posizione caso continuo 
};

void evolvi(Random& rnd, pos& posiz) {       //evoluzione nel caso discreto
    double a = rnd.Rannyu();
    if (a<0.5) a = -1;
        else a = 1;                          //estraggo con uguale probabilità -1 o 1
    int direz = int(rnd.Rannyu(0,3));
    posiz.posizione[direz] += int(a);
};

void evolvi_c(Random& rnd, pos_c& posiz) {              //evoluzione nel caso continuo
    double theta = acos(1-2*rnd.Rannyu());              //estraggo theta dall'inversa della cumulativa
    double phi = rnd.Rannyu(0, 2*M_PI);
    posiz.posizione_c[0] += sin(theta)*cos(phi);
    posiz.posizione_c[1] += sin(theta)*sin(phi);
    posiz.posizione_c[2] += cos(theta);
};

double dist(pos& posiz) {                           //calcolo la distanza dall'origine
    double sum = 0;
    for (int i=0; i<3; i++) sum += posiz.posizione[i]*posiz.posizione[i];
    return sum;
};

double dist_c(pos_c& posiz) {
    double sum = 0;
    for (int i=0; i<3; i++) sum += posiz.posizione_c[i]*posiz.posizione_c[i];
    return sum;
};

double error (double av2, double av, int n) {
    if (n==0) return 0;
    return sqrt((av2-av*av)/n);
};

int main() {

    Random rnd;
    rnd.setAll(rnd);
    int M = 10000;      //# simulazioni
    int L = 100;        //lunghezza blocchi
    int N = M/L;        //# blocchi

    pos percorso[M] = {};        //array delle posizioni che voglio far evolvere
    pos_c percorso_c[M] = {};    //uguale nel continuo

    ofstream distanza("Distanza.txt");
    ofstream distanza_c("Distanza_c.txt");

    double ave[N];             //array dei valori medi di ciascun blocco
    double ave2[N];            //array dei valori medi al quadrato di ciascun blocco
    double ave_c[N];           //uguale nel continuo
    double ave2_c[N];          //uguale nel continuo

    distanza << 0 << " " << 0 << " " << 0 << endl;          //distanza al passo 0
    distanza_c << 0 << " " << 0 << " " << 0 << endl;        //continuo

    for (int j=1; j<=100; j++) {
        double somma_blocchi = 0;       //somme dei valori medi di tutti i blocchi
        double somma_blocchi2 = 0;      //somme dei valori medi al quadrato di tutti i blocchi
        double somma_blocchi_c = 0;
        double somma_blocchi2_c = 0;
        
        for (int i=0; i<N; i++) {
            double sum = 0;             //somme delle distanze all'interno di ciascun blocco
            double sum_c = 0;           //continuo

            for (int k=0; k<L; k++) {
                evolvi(rnd, percorso[i*N+k]);           //evolvo il random walk di ciascuna simulazione
                evolvi_c(rnd, percorso_c[i*N+k]);       //contiuo
                sum += dist(percorso[i*N+k]);           
                sum_c += dist_c(percorso_c[i*N+k]);     
            };
        
            ave2[i] = sum/L;
            ave[i] = sqrt(sum/L);
            somma_blocchi += ave[i];
            somma_blocchi2 += ave2[i];
            ave2_c[i] = sum_c/L;
            ave_c[i] = sqrt(sum_c/L);
            somma_blocchi_c += ave_c[i];
            somma_blocchi2_c += ave2_c[i];
        }

        distanza << j << " " << somma_blocchi/N << " " << error(somma_blocchi2/N, somma_blocchi/N, N-1) << endl;
        distanza_c << j << " " << somma_blocchi_c/N << " " << error(somma_blocchi2_c/N, somma_blocchi_c/N, N-1) << endl;
    };

    distanza.close();
    distanza_c.close();
    return 0;
}