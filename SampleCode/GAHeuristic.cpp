//
// Created by wmw13 on 19/02/2021.
//

#include "GAHeuristic.h"

/*
 * Creates and runs a Genetic Algorithm.
 */
void GAHeuristic(){
    int sizeOfPopulation = 10, generations = 50;
    auto* GA = new GeneticAlgorithm(sizeOfPopulation,generations);
    GA->generateStartingPopulation();
    GA->displayPopulation();

    ////FORCE STOP THE PROGRAM AFTER ONE ITERATION.
    int a[10] = {5,4,2,3,6,7,4,4,7,6};
    check_solution(a,10);
    ////////////////////////////////////////////

    delete GA;
}
