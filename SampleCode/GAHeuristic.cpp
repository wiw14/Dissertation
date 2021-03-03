//
// Created by wmw13 on 19/02/2021.
//

#include "GAHeuristic.h"

/*
 * Creates and runs a Genetic Algorithm.
 */
void GAHeuristic() {

    int sizeOfPopulation = 10, generations = 5;
    auto *GA = new GeneticAlgorithm(sizeOfPopulation, generations);
    GA->generateStartingPopulation();

    GA->runGenerations();
//    ////FORCE STOP THE PROGRAM AFTER ONE ITERATION.
    int a[10] = {5, 4, 2, 3, 6, 7, 4, 4, 7, 6};
    check_solution(a, 10);
//    ////////////////////////////////////////////

    //DEBUGGING
//    GA->displayPopulation();

    GA->checkSolution();
    delete GA;
}
