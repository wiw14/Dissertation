#include<string>

#include "ACOWithChargingStation.h"
#include "ACO.h"


void ACOHeuristic(){
    int numAnts=3, iterations = 350, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.8, Q = 80,alpha = 0.8, beta=0.8;
    auto* a = new ACO(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta,twoOptIteration,randomSearchIteration);
    //printf("ACO Initialised\n"); //DEBUGGING
    a->optimize(iterations);
    //printf("ACO Optimized\n"); //DEBUGGING
    int * route = a->returnResults();
    //DEBUGGING
//    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//        printf("%d, ",route[index]);
//    printf("\n");
    a->getRL(route);
    delete a;
}
void ACOCSHeuristic(){
    int numAnts=3, iterations = 1000, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.8, Q = 80,alpha = 0.8, beta=0.8;
    auto* a = new ACOCS(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta,twoOptIteration,randomSearchIteration);
    //printf("ACO Initialised\n"); //DEBUGGING
    a->optimize(iterations);
    //printf("ACO Optimized\n"); //DEBUGGING
    int * route = a->returnResults();
    //DEBUGGING
//    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//        printf("%d, ",route[index]);
//    printf("\n");
    a->getRL(route);
    delete a;
}