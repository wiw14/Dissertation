#include "ACOv1.h"
#include "EVRP.hpp"

#include <cstdio>
#include <iostream>
#include <cstdlib>

#include <cmath>
#include <limits>
#include <climits>

using namespace std;

ACOv1::ACOv1 (int nAnts, int numCustomers,
              double alpha, double beta, double q, double ro, double taumax,
              int startDepot) {
    numberOfAnts 	= nAnts;
    numberOfCustomers 	= numCustomers;
    Alpha 			= alpha;
    Beta 			= beta;
    Q 				= q;
    RO 				= ro;
    Taumax 			= taumax;
    depot		= startDepot;

    randoms = new Randoms (21);
}
ACOv1::~ACOv1 () {
    for(int i=0; i < numberOfCustomers; i++) {
        delete [] customers[i];
        delete [] pheromones[i];
        delete [] deltaPheromones[i];
        if(i < numberOfCustomers - 1) {
            delete [] probability[i];
        }
    }
    delete [] customers;
    delete [] pheromones;
    delete [] deltaPheromones;
    delete [] probability;
}

void ACOv1::init () {
    customers 			= new double*[numberOfCustomers];
    pheromones 		= new double*[numberOfCustomers];
    deltaPheromones = new double*[numberOfCustomers];
    probability 			= new double*[numberOfCustomers - 1];
    for(int i=0; i < numberOfCustomers; i++) {
        customers[i] 			= new double[2];
        pheromones[i] 		= new double[numberOfCustomers];
        deltaPheromones[i] 	= new double[numberOfCustomers];
        probability[i] 			= new double[2];
        for (int j=0; j<2; j++) {
            customers[i][j] = -1.0;
            probability[i][j]  = -1.0;
        }
        for (int j=0; j < numberOfCustomers; j++) {
            pheromones[i][j] 		= 0.0;
            deltaPheromones[i][j] 	= 0.0;
        }
    }

    routes = new int*[numberOfAnts];
    for (int i=0; i < numberOfAnts; i++) {
        routes[i] = new int[numberOfCustomers];
        for (int j=0; j < numberOfCustomers; j++) {
            routes[i][j] = -1;
        }
    }

    bestLength = (double) INT_MAX;
    bestRoute  = new int[numberOfCustomers];
    for (int i=0; i < numberOfCustomers; i++) {
        bestRoute[i] = -1;
    }
}


void ACOv1::connectCustomers (int cityi, int cityj) {
    //graph[cityi][cityj] = 1;
    pheromones[cityi][cityj] = randoms -> Uniforme() * Taumax;
    //graph[cityj][cityi] = 1;
    pheromones[cityj][cityi] = pheromones[cityi][cityj];
}
void ACOv1::setCustomerLocation (int city, double x, double y) {
    customers[city][0] = x;
    customers[city][1] = y;
}
//void ACOv1::printPHEROMONES () {
//    cout << " pheromones: " << endl;
//    cout << "  | ";
//    for (int i=0; i < numberOfCustomers; i++) {
//        printf("%5d   ", i);
//    }
//    cout << endl << "- | ";
//    for (int i=0; i < numberOfCustomers; i++) {
//        cout << "--------";
//    }
//    cout << endl;
//    for (int i=0; i < numberOfCustomers; i++) {
//        cout << i << " | ";
//        for (int j=0; j < numberOfCustomers; j++) {
//            if (i == j) {
//                printf ("%5s   ", "x");
//                continue;
//            }
//            if (exists(i, j)) {
//                printf ("%7.3f ", pheromones[i][j]);
//            }
//            else {
//                if(pheromones[i][j] == 0.0) {
//                    printf ("%5.0f   ", pheromones[i][j]);
//                }
//                else {
//                    printf ("%7.3f ", pheromones[i][j]);
//                }
//            }
//        }
//        cout << endl;
//    }
//    cout << endl;
//}

double ACOv1::distance (int customerA, int customerB) {
    return get_distance(customerA,customerB);
}

bool ACOv1::exists (int customerA, int customerB) {
    return (get_distance(customerA,customerB) > 0);
}

bool ACOv1::visited (int antk, int c) {
    for (int l=0; l < numberOfCustomers; l++) {
        if (routes[antk][l] == -1) {
            break;
        }
        if (routes[antk][l] == c) {
            return true;
        }
    }
    return false;
}
double ACOv1::PHI (int customerA, int customerB, int antk) {
    double ETAij = (double) pow (1 / distance (customerA, customerB), Beta);
    double TAUij = (double) pow (pheromones[customerA][customerB], Alpha);

    double sum = 0.0;
    for (int c=0; c < numberOfCustomers; c++) {
        if (exists(customerA, c)) {
            if (!visited(antk, c)) {
                double ETA = (double) pow (1 / distance (customerA, c), Beta);
                double TAU = (double) pow (pheromones[customerA][c], Alpha);
                sum += ETA * TAU;
            }
        }
    }
    return (ETAij * TAUij) / sum;
}

double ACOv1::length (int antk) {
    double sum = 0.0;
    for (int j=0; j < numberOfCustomers - 1; j++) {
        sum += distance (routes[antk][j], routes[antk][j + 1]);
    }
    return sum;
}

int ACOv1::customer () {
    double xi = randoms -> Uniforme(); //gets a random number
    int i = 0;
    double sum = probability[i][0]; //sets the probability
    while (sum < xi) { //loops until the sum of probability is less than the randomly selected number.
        i++;
        sum += probability[i][0];
    }
    return (int) probability[i][1]; //The customer when the loop ends is selected as the next customer in the route.
}

void ACOv1::route (int antk) {
    routes[antk][0] = depot;
    for (int i=0; i < numberOfCustomers - 1; i++) {
        int customerA = routes[antk][i];
        int count = 0;
        for (int c=0; c < numberOfCustomers; c++) {
            if (customerA == c) {
                continue;
            }
            if (exists (customerA, c)) {
                if (!visited(antk, c)) {
                    probability[count][0] = PHI (customerA, c, antk);
                    probability[count][1] = (double) c;
                    count++;
                }

            }
        }

        // deadlock
        if (0 == count) {
            return;
        }

        routes[antk][i + 1] = customer();
    }
}
int ACOv1::valid (int antk, int iteration) {
    for(int i=0; i < numberOfCustomers - 1; i++) {
        int customerA = routes[antk][i];
        int customerB = routes[antk][i + 1];
        if (customerA < 0 || customerB < 0) {
            return -1;
        }
        if (!exists(customerA, customerB)) {
            return -2;
        }
        for (int j=0; j<i-1; j++) {
            if (routes[antk][i] == routes[antk][j]) {
                return -3;
            }
        }
    }

    if (!exists (depot, routes[antk][numberOfCustomers - 1])) {
        return -4;
    }

    return 0;
}

//void ACOv1::printGRAPH () {
//    cout << " graph: " << endl;
//    cout << "  | ";
//    for(int i=0; i < numberOfCustomers; i++) {
//        cout << i << " ";
//    }
//    cout << endl << "- | ";
//    for (int i=0; i < numberOfCustomers; i++) {
//        cout << "- ";
//    }
//    cout << endl;
//    int count = 0;
//    for (int i=0; i < numberOfCustomers; i++) {
//        cout << i << " | ";
//        for (int j=0; j < numberOfCustomers; j++) {
//            if(i == j) {
//                cout << "x ";
//            }
//            else {
//                cout << graph[i][j] << " ";
//            }
//            if (graph[i][j] == 1) {
//                count++;
//            }
//        }
//        cout << endl;
//    }
//    cout << endl;
//    cout << "Number of connections: " << count << endl << endl;
//}
void ACOv1::printRESULTS () {
    bestLength += distance (bestRoute[numberOfCustomers - 1], depot);
    cout << " BEST ROUTE:" << endl;
    for (int i=0; i < numberOfCustomers; i++) {
        cout << bestRoute[i] << " ";
    }
    cout << endl << "length: " << bestLength << endl;

}

int* ACOv1::returnResults(){
    return bestRoute;
}

void ACOv1::updatePHEROMONES () {
    for (int k=0; k < numberOfAnts; k++) {
        double rlength = length(k);
        for (int r=0; r < numberOfCustomers - 1; r++) {
            int customerA = routes[k][r];
            int customerB = routes[k][r + 1];
            deltaPheromones[customerA][customerB] += Q / rlength;
            deltaPheromones[customerB][customerA] += Q / rlength;
        }
    }
    for (int i=0; i < numberOfCustomers; i++) {
        for (int j=0; j < numberOfCustomers; j++) {
            pheromones[i][j] = (1 - RO) * pheromones[i][j] + deltaPheromones[i][j];
            deltaPheromones[i][j] = 0.0;
        }
    }
}


void ACOv1::optimize (int ITERATIONS) {
    for (int iterations=1; iterations<=ITERATIONS; iterations++) {
        for (int k=0; k < numberOfAnts; k++) {
            while (0 != valid(k, iterations)) {
                for (int i=0; i < numberOfCustomers; i++) {
                    routes[k][i] = -1;
                }
                route(k);
            }
            double rlength = length(k);

            if (rlength < bestLength) {
                bestLength = rlength;
                for (int i=0; i < numberOfCustomers; i++) {
                    bestRoute[i] = routes[k][i];
                }
            }
        }

        updatePHEROMONES ();
        for (int i=0; i < numberOfAnts; i++) {
            for (int j=0; j < numberOfCustomers; j++) {
                routes[i][j] = -1;
            }
        }

    }
}