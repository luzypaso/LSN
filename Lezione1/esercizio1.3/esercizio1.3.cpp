#include <iostream>
#include <cmath>
#include <fstream>
#include "random.h"

using namespace std;

double error(double av, double av2, int n) {     //Function for statistical uncertainty estimation
    if (n==0) {
        return 0;
    } else {
        return sqrt((av2 - pow(av, 2))/n);
    };
};

double stima_pi(Random& rnd){
    int N = 10000;
    int sum = 0;

    double temp;
    for(int i=0; i<N; i++){
        temp = pow(rnd.Rannyu(), 2) + pow(rnd.Rannyu(), 2);   //temp è pari alla distanza dall'origine di un punto con coord x e y comprese tra (0, 1)
        if(temp<1) sum++;                                     //sum quantifica quante volte il punto estratto appartiene all'area del cerchio unitario
    }
    return double(sum)*4./double(N);                          //il rapporto tra l'area di un quadrato e del cerchio inscritto è pari a 4/pi
};

int main() {
    Random rnd;
    rnd.setAll(rnd);

    double l = 1;         //Length of the noodle
    double d = 1.5;       //Distance bewteen lines

    int M = 100000;
    int L = 1000;
    int N = M/L;
    double values[N];
    double values2[N];
    double ave;
    double sum;
    int counts;
    double pi = stima_pi(rnd);
    
    for (int i=0; i<N; i++) {
        counts = 0;
        double x;               //punto medio sbarra
        double x2;              //distanza dell'estremità superiore rispetto al punto medio

        for (int j=0; j<L; j++) {
            x = rnd.Rannyu(0, d/2);
            x2 = l/2*sin(rnd.Rannyu()*pi);
            if (x<=x2) counts ++;
        };        

        values[i] = (2*l*L)/(d*counts);
        values2[i] = values[i]*values[i];
    };

    ofstream Buffon("Buffon.txt");
    double prog;
    double prog2;

    for (int i=1; i<N+1; i++) {
        prog=0;
        prog2=0;
        for (int j=0; j<i; j++) {
            prog += values[j];
            prog2 += values2[j];
        };
        prog = prog/i;
        prog2 = prog2/i;
        Buffon << i << " " << prog << " " << error(prog, prog2, i) << endl;
    };

    Buffon.close();
    return 0;
}