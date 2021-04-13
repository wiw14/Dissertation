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
void ACOHeuristic(int run){
    int numAnts= 4, iterations = 20, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.98, Q = 1,alpha = 0.9, beta=0.9;
    if(run == 1)
        printf("ANT COLONY OPTIMISATION\nNum Ants: %d\nIterations: %d\n2-Opt Iterations: %d\nExchange Search Iterations: %d\nPheromone Decrease: %f\nQ: %f\nAlpha: %f\nBeta: %f\n\n",numAnts,iterations,twoOptIteration,randomSearchIteration,pheromoneDecrease,Q,alpha,beta);

    auto* a = new ACO(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta,twoOptIteration,randomSearchIteration);

    a->optimize(iterations);
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
void ACOCSHeuristic(int run){
    int numAnts=3, iterations = 30, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.8, Q = 80,alpha = 0.8, beta=0.8;
    if (run == 1)
        printf("ANT COLONY OPTIMISATION WITH CHARGING STATIONS\nNum Ants: %d\nIterations: %d\n2-Opt Iterations: %d\nExchange Search Iterations: %d\nPheromone Decrease: %f\nQ: %f\nAlpha: %f\nBeta: %f\n\n",numAnts,iterations,twoOptIteration,randomSearchIteration,pheromoneDecrease,Q,alpha,beta);
    auto* a = new ACOCS(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta,twoOptIteration,randomSearchIteration);
    a->optimize(iterations);
    int * route = a->returnResults();
    a->getRL(route);
    delete a;
}

void MMACOHeuristic(int run){
    int numAnts=8, iterations = 50, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.9, Q = 1,alpha = 0.6, beta=2.1, pBest = 0.05;
    if (run == 1)
        printf("MAX MIN ANT COLONY OPTIMISATION\nNum Ants: %d\nIterations: %d\n2-Opt Iterations: %d\nExchange Search Iterations: %d\nPheromone Decrease: %f\nQ: %f\nAlpha: %f\nBeta: %f\n\n",numAnts,iterations,twoOptIteration,randomSearchIteration,pheromoneDecrease,Q,alpha,beta);
    auto* a = new MaxMinACO(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha, pBest,beta,twoOptIteration,randomSearchIteration);
    a->optimize(iterations);
    int * route = a->returnResults();
    a->getRL(route);
    delete a;
}