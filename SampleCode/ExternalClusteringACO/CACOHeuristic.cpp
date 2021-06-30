#include "CACOHeuristic.h"
#include "CACO.h"
/*
 * Creates and configures CACO object using the created variables.
 * Runs optimise function to find a local optimum.
 * Adds local optimum as best solution.
 *
 * CACO updates pheromones based on length of  total route of customers, charging stations, and depots
 * rather than just the route of the customers.
 */
void CACOHeuristic(int nAnts, int iter, int probSize, double pheroDec, double q, double al, double be, int rsi, int toi){

    auto* a = new CACO(nAnts,pheroDec,q,probSize,al,be,toi,rsi);

    a->optimize(iter);
    int * route = a->returnResults();

    a->getRL(route);

    delete a;
}

