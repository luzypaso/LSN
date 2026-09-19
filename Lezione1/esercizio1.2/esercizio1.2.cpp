#include <iostream>
#include <fstream>
#include <cmath>
#include "random.h"

using namespace std;

int main() {
    Random rnd;
    rnd.setAll(rnd);
    int N[4] = {1, 2, 10, 100};
    double sum;

    ofstream Uniform("Uniform.txt");
    ofstream Expo("Expo.txt");
    ofstream Lorentz("Lorentz.txt");

    for (int i=0; i<10000; i++) {
        
        for (int k=0; k<4; k++) {
            sum = 0;
            
            for (int j=0; j<N[k]; j++) {
            sum += rnd.Rannyu();
            }
            Uniform << sum/N[k] << " ";
        }
        Uniform << endl;
    }
    cout << "File 'Uniform.txt' created" << endl;

    for (int i=0; i<10000; i++) {
        
        for (int k=0; k<4; k++) {
            sum = 0;
            
            for (int j=0; j<N[k]; j++) {
            sum += rnd.Expo(1.);
            }
            Expo << sum/N[k] << " ";
        }
        Expo << endl;
    }
    cout << "File 'Expo.txt' created" << endl;

    for (int i=0; i<10000; i++) {
        
        for (int k=0; k<4; k++) {
            sum = 0;
            
            for (int j=0; j<N[k]; j++) {
            sum += rnd.Lorentz(1., 0.);
            }
            Lorentz << sum/N[k] << " ";
        }
        Lorentz << endl;
    }
    cout << "File 'Lorentz.txt' created" << endl;

    Uniform.close();
    Expo.close();
    Lorentz.close();
    return 0;
}