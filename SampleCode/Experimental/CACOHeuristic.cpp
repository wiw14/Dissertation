//
// Created by wmw13 on 05/03/2021.
//

#include "CACOHeuristic.h"
#include "CACO.h"
void CACOHeuristic(){
    int numAnts= 3, iterations = 20, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.1, Q = 1,alpha = 0.6, beta=2.1;
    auto* a = new CACO(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta,twoOptIteration,randomSearchIteration);

    a->optimize(iterations);
    int * route = a->returnResults();

    a->getRL(route);

    delete a;
}