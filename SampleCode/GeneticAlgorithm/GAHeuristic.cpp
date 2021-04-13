//
// Created by wmw13 on 19/02/2021.
//

#include "GAHeuristic.h"

/*
 * Creates and runs a Genetic Algorithm.
 */
void GAHeuristic(int run) {
    int sizeOfPopulation = 15, generations = 20, numMutations = 10;
    if(run == 1)
        printf("GENETIC ALGORITHM\nSize of Population: %d\nGenerations: %d\nProbability of Mutation: 1/%d\n\n",sizeOfPopulation,generations,numMutations);
    auto *GA = new GeneticAlgorithm(sizeOfPopulation, generations,numMutations);
    GA->generateStartingPopulation();
//    printf("Before Generations\n");
    GA->runGenerations();
//    ////FORCE STOP THE PROGRAM AFTER ONE ITERATION.
//    int a[10] = {5, 4, 2, 3, 6, 7, 4, 4, 7, 6};
//    check_solution(a, 10);
//    ////////////////////////////////////////////

    //DEBUGGING
//    GA->displayPopulation();

    GA->checkSolution();

    delete GA;
}
