#include <cmath>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include "system.h"

using namespace std;

Individual :: Individual() {
    for (int i=0; i<_ncities; i++) {            //genero inizialmente le citta` ordinate da 0 a 33 in modo da non avere doppioni
        _cities[i] = i;
    }
}

void Individual :: check() {
    vector<bool> sonda(_ncities, false);        //Uso di vector<bool> come tabella "sonda" per verificare la presenza di duplicati.

    for (int i=0; i<_ncities; i++) {
        int c = getcity(i);
        if (c < 0 || c >= _ncities) {           //Controllo che le città abbiano valori tra 0 e 33
            cout << "ERRORE: trovata città con valore " << c << endl;
            exit(1);
        }
        if (sonda[c]) {                         //Controllo doppioni
            cout << "ERRORE: trovato doppione con valore " << c << endl;
            exit(1);
        }
        sonda[c] = true;
    }
    if (getcity(0) != 0) {                      //Controllo che la prima città sia sempre la 0
        for (int i=1; i<_ncities; i++) {
            if (getcity(i)==0) {
                int temp = getcity(0);
                setcity(0, 0);
                setcity(i, temp);
            }
        }
    }
}
    

Population :: Population(City cities[]) {
    rnd.setAll(rnd);

    for(int i=0; i<_n; i++) _class_cities[i] = cities[i];               //Salvataggio delle coordinate di tutte le città
    
    //random_device rd;                                                   //Generatore casuale C++11 per il rimescolamento iniziale
    //mt19937 g(rd());

    for (int j=0; j<_N; j++) {
        _individual[j].shufflecities(rnd);                                //Mescola gli elementi
        if (_individual[j].getcity(0) != 0) {                           //check ed eventuale correzione del primo termine dell'individuo (che dev'essere uguale a 0)
            for (int i=1; i<_individual[j].gettotcities(); i++) {
                if (_individual[j].getcity(i) == 0) {
                    int temp = _individual[j].getcity(0);
                    _individual[j].setcity(0, 0);
                    _individual[j].setcity(i, temp);
                    break;
                }
            }
        }
    }
}

Population :: Population(City cities[], int rank) {
    rnd.setAll(rnd, rank);

    for(int i = 0; i < _n; i++) _class_cities[i] = cities[i];

    //random_device rd;                                                   //Generatore casuale C++11 per il rimescolamento iniziale
    //mt19937 g(rd());

    for (int j = 0; j < _N; j++) {
        _individual[j].shufflecities(rnd);
        if (_individual[j].getcity(0) != 0) {                           //check ed eventuale correzione del primo termine dell'individuo (che dev'essere uguale a 0)
            for (int i=1; i<_individual[j].gettotcities(); i++) {
                if (_individual[j].getcity(i) == 0) {
                    int temp = _individual[j].getcity(0);
                    _individual[j].setcity(0, 0);
                    _individual[j].setcity(i, temp);
                    break;
                }
            }
        }
    }
}

void Individual :: shufflecities(Random& rnd) {

    for (int i=_ncities-1; i>1; i--) {
        int j = 1 + int(rnd.Rannyu()*i);
        int temp = getcity(i);
        setcity(i, getcity(j));
        setcity(j, temp);
    }
}

void Individual :: calculatecost(const City cities[]) {

    double sum = 0;

    for(int i=0; i<_ncities-1; i++) {                                   //calcolo distanza tra le città successive
        double dx = cities[_cities[i]].x - cities[_cities[i+1]].x;
        double dy = cities[_cities[i]].y - cities[_cities[i+1]].y;
        sum += sqrt(dx*dx + dy*dy);
    }
    double dx = cities[_cities[_ncities-1]].x - cities[_cities[0]].x;   //aggiungo il calcolo distanza tra l'ultima città e la prima
    double dy = cities[_cities[_ncities-1]].y - cities[_cities[0]].y;
    sum += sqrt(dx*dx + dy*dy);
    _cost = sum;
}

void Population :: ordering() {
    for (int i=0; i<_N; i++) {
        _individual[i].calculatecost(_class_cities);             //calcolo il costo di tutti gli individui
    }
    for (int i=0; i<_N-1; i++) {                                 //ordino la popolazione in base al costo minore
        for (int j=i+1; j<_N; j++) {
            if (_individual[i].getcost() > _individual[j].getcost()) {
                Individual temp = _individual[j];
                _individual[j] = _individual[i]; 
                _individual[i] = temp;
            }
        }
    }
}

int Population :: selection() {
    int indiv = int(_N * pow(rnd.Rannyu(), 3));             //estraggo un indivduo a caso con una probabilità maggiore di estrarre un individuo
    return indiv;                                           //che presenta un indice basso, sfruttando la potenza cubica di un numero compreso tra 0 e 1
}

Individual Population :: pair_permutation(Individual ind) {         //scambio di due città adiacenti
    Individual ind2 = ind;
    int s = int(rnd.Rannyu(1, ind.gettotcities()-1));               //non prendo la prima città che deve restare fissa
    if (s==ind.gettotcities()-1) {                                  //se seleziono l'ultima città la scambio con la seconda
        ind2.setcity(s, ind.getcity(1));
        ind2.setcity(1, ind.getcity(s));
    } else {
        ind2.setcity(s, ind.getcity(s+1));
        ind2.setcity(s+1, ind.getcity(s));
    }
    return ind2;
}

Individual Population :: contig_permutation(Individual ind) {                   //shift/scorrimento di un blocco di città
    Individual ind2 = ind;
    int s = int(rnd.Rannyu(1, ind.gettotcities()-2));                           //seleziono casualmente la prima città del blocco tra 1 e 32
    if ((ind.gettotcities()-s)<1) return ind2;                                  //check che la prima città sia entro i confini 1-32
    int m = int(rnd.Rannyu(1, ind.gettotcities()-s));                           //seleziono casualmente quante città nel blocco da shiftare
    int temp = ind.gettotcities() - (s+m);                                      //temp è il numero di città che restano tra la fine del blocco e la fine dell'array
    if (temp<1) return ind2;                                                    //controllo che il blocco non arrivi già in fondo all'array
    int n = int(rnd.Rannyu(1, temp+1));                                         //seleziono casualmente di quanto shifto
                                                                                //n indica quante città tra la fine del blocco e la fine dell'array verrano scavalcate 
    rotate(ind2.getcity_s()+s, ind2.getcity_s()+s+m, ind2.getcity_s()+s+m+n);   //il blocco di m città avanza scavalcando n posizioni

    return ind2;
}


Individual Population :: contig_contig_permutation(Individual ind) {            //scambio tra due blocchi contigui
    Individual ind2 = ind;

    int s1 = int(rnd.Rannyu(1, ind2.gettotcities()/2));                         //seleziono casualmente la prima città del primo blocco, che prendo nella prima metà dell'array
    if((ind2.gettotcities()/2-s1)<1) return ind2;
    int m = int(rnd.Rannyu(1, ind2.gettotcities()/2-s1));                       //seleziono casualmente quante città permutare
    int s2 = int(rnd.Rannyu(ind2.gettotcities()/2, ind2.gettotcities()-1-m));   //seleziono casualmente la prima città del secondo blocco, che prendo nella seconda metà dell'array

    for(int i=0; i<m; i++) {
        ind2.setcity(s1+i, ind.getcity(s2+i));
        ind2.setcity(s2+i, ind.getcity(s1+i));
    }

    return ind2;
}

Individual Population :: inversion(Individual ind) {            //inversione di un blocco di città
    Individual ind2 = ind;
    int ncities = ind.gettotcities();
    
    int s1 = int(rnd.Rannyu(1, ncities));                       //seleziono due indici casuali come estremi del blocco
    int s2 = int(rnd.Rannyu(1, ncities));
    while (s1 == s2) {                                          //check se i due indici combaciano
        s2 = int(rnd.Rannyu(1, ncities));
    }
    if (s1 > s2) {                                              //assicuro che s1 sia l'indice iniziale e s2 quello finale
        swap(s1, s2);
    }

    while (s1 < s2) {                                           //inverto l'ordine delle città
        int temp = ind2.getcity(s1);
        ind2.setcity(s1, ind2.getcity(s2));
        ind2.setcity(s2, temp);
        s1++;
        s2--;
    }

    return ind2;
}

Individual Population :: crossover(Individual ind1, Individual ind2) {      //crossover tra due individui
    Individual ind3 = ind1;                                                 //inizialmente il figlio è uguale a ind1
    int s = int(rnd.Rannyu(2, ind1.gettotcities()-1));                      //seleziono casualmente il punto di taglio
    
    vector<bool> sonda(ind1.gettotcities(), false);                         //sonda per verificre doppioni o lacune di città
    for (int i=0; i<s; i++) {
        sonda[ind1.getcity(i)] = true;                                      //valore true per le città del primmo blocco dell'ind1
    }
    
    ind1.check();
    ind2.check();
    for (int i=1; i<ind1.gettotcities(); i++) {
        int temp = ind2.getcity(i);

        if (!sonda[temp]) {                                                //riempio la restante parte del figlio (dal taglio s in poi) con le città
            ind3.setcity(s, temp);                                         //che non compaiono nella prima metà e lo faccio seguendo l'ordine in cui compaiono in ind2
            s++;                                                           //in altre parole: se la città (i) di ind2 non è già presente nelle prime 's' posizioni del figlio
            sonda[temp] = true;                                            //aggiungo la città nel primo posto libero da s in poi
        }
    }

    return ind3;
}

