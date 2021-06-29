#include "GAHeuristic.h"

/*
 * Creates and runs the Genetic Algorithm.
 */
void GAHeuristic(int popSize, int gen, int numMut,int rsi,int toi,int selection,int crossover, int mutation) {

    auto *GA = new GeneticAlgorithm(popSize, gen,numMut,rsi,toi);

    GA->generateStartingPopulation(crossover,selection);
    GA->runGenerations(selection,crossover,mutation);

    //DEBUGGING
//    GA->displayPopulation();

    GA->checkSolution();
    delete GA;
}
