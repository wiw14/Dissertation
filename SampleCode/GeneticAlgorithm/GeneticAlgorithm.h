//
// Created by wmw13 on 19/02/2021.
//

#ifndef TESTSAMPLECODE_GENETICALGORITHM_H
#define TESTSAMPLECODE_GENETICALGORITHM_H

#include "../GeneticAlgorithm/CrossoverOperators.h"
#include <deque>
#include <list>

class GeneticAlgorithm {
private:
    int sizeOfPopulation,childPopulationCounter;
    int generations;
    int numMutations;
    int** parentPopulation;
    int** childPopulation;
    localSearch* LS;

    void randomRoute(int*);
    static void deleteSegmentOfArray(int**, int, int);
    std::pair<int*,int> getBestRoute();
    void selectChildrenForParents();
    void repairParents();
    void crossoverOperator();

public:
    GeneticAlgorithm(int,int,int);
    virtual ~GeneticAlgorithm();
    void generateStartingPopulation();
    void runGenerations();
    void displayPopulation();
    void checkSolution();
};


#endif //TESTSAMPLECODE_GENETICALGORITHM_H
