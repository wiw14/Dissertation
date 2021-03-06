#include "CACOHeuristic.h"
#include "CACO.h"
/*
 * Creates and initialises Ant Colony to run on the clusters generated from the initial clusters.
 */
void CACOHeuristic(){
    int numAnts= 3, iterations = 20, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.1, Q = 1,alpha = 0.6, beta=2.1;
    auto* a = new CACO(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta,twoOptIteration,randomSearchIteration);

    a->optimize(iterations);
    int * route = a->returnResults();

    a->getRL(route);

    delete a;
}