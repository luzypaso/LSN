#ifndef __System__
#define __System__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h> //exit
#include <algorithm>   // std::shuffle
#include <random>      // std::random_device, std::mt19937
#include "random.h"

using namespace std;

const int _n = 34;               //valore costante del numero di città per individuo
const int _np = 50;              //valore costante del numero di individui per popolazione

struct City {
    double x;
    double y;
};

class Individual {

    private:
    int _ncities = _n;
    int _cities[_n];             //vettore contenente l'ordine delle citta`
    double _cost;

    
    public:
    Individual();

    //Public methods
    void shufflecities(mt19937& g) {shuffle(_cities, _cities + _ncities, g);};
    int gettotcities() {return _ncities;};
    int getcity(int i) {return _cities[i];};
    int* getcity_s() {return _cities;};                 //puntatore all'array di _cities, mi serve per la funzione rotate
    void setcity(int i, int j) {_cities[i] = j;};
    double getcost() {return _cost;};
    void calculatecost(const City cities[]);
    void check();
    
};


class Population {

    private:
    int _N = _np;                          //# individui
    Individual _individual[_np];
    City _class_cities[_n];                //array per salvare le coordinate di tutte le città
    Random rnd;
    
    public:
    Population(City cities[]);
    
    //Public methods
    int get_n() {return _N;}
    void ordering();
    int selection();
    Individual& getindividual(int i) {return _individual[i];};             //devo restituire un riferimento, non una copia, mi serve per il main
    Individual pair_permutation(Individual ind);
    Individual contig_permutation(Individual ind);
    Individual contig_contig_permutation(Individual ind);                  
    Individual inversion(Individual ind);
    Individual crossover(Individual ind1, Individual ind2);               
    void check();                                                          //metodo di check
};


#endif // __System__