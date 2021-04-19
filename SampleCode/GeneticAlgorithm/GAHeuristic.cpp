#include "GAHeuristic.h"

/*
 * Creates and runs the Genetic Algorithm.
 */
void GAHeuristic() {
    int sizeOfPopulation = 20, generations = 50, numMutations = 10;
    auto *GA = new GeneticAlgorithm(sizeOfPopulation, generations,numMutations);

    GA->generateStartingPopulation();
    GA->runGenerations();

    //DEBUGGING
//    GA->displayPopulation();

    GA->checkSolution();
    delete GA;
}
