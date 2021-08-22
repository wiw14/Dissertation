#include<string>

#include "MaxMinACO.h"
#include "ACOWithChargingStation.h"
#include "ACO.h"

/*
 * Creates and configures AntColonyOptimisation object using the created variables.
 * Runs optimise function to find a local optimum.
 * Adds local optimum as best solution.
 *
 * AntColonyOptimisation updates pheromones based on length of customer route rather than total route of customers,
 * charging stations, and depots.
 */
void ACOHeuristic(int nAnts, int iter, int probSize, double pheroDec, double q, double al, double be, int rsi, int toi){

    auto* a = new ACO(nAnts,pheroDec,q,probSize,al,be,toi,rsi);

    a->optimize(iter);

    int * route = a->returnResults();
    a->getRL(route);

    delete a;
}

/*
 * Creates and configures ACOCS object using the created variables.
 * Runs optimise function to find a local optimum.
 * Adds local optimum as best solution.
 *
 * ACOCS updates pheromones based on length of  total route of customers, charging stations, and depots
 * rather than just the route of the customers.
 */
void ACOCSHeuristic(int nAnts, int iter, int probSize, double pheroDec, double q, double al, double be, int rsi, int toi){

    auto* a = new ACOCS(nAnts,pheroDec,q,probSize,al,be,toi,rsi);

    a->optimize(iter);

    int * route = a->returnResults();
    a->getRL(route);

    delete a;
}

/*
 * Creates and configures MMACO object using the created variables.
 * Runs optimise function to find a local optimum.
 * Adds local optimum as best solution.
 *
 * MMACO updates pheromones based on length of  total route of customers, charging stations, and depots
 * rather than just the route of the customers.
 */
void MMACOHeuristic(int nAnts, int iter, int probSize, double pheroDec, double q, double al, double be, int rsi, int toi,double pb){

    double pBest = pb;

    auto* a = new MaxMinACO(nAnts,pheroDec,q,probSize,al, pBest,be,toi,rsi);

    a->optimize(iter);

    int * route = a->returnResults();
    a->getRL(route);

    delete a;
}