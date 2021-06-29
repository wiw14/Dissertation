#include "GAHeuristic.h"

/*
 * Creates and runs the Genetic Algorithm.
 */
void GAHeuristic() {
    int sizeOfPopulation = 10, generations = 10, numMutations = 5;
    auto *GA = new GeneticAlgorithm(sizeOfPopulation, generations,numMutations);

    GA->generateStartingPopulation();
    GA->runGenerations();

    //DEBUGGING
//    GA->displayPopulation();

    GA->checkSolution();
    delete GA;
}
