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
    int generations, probabiltyOfMutation;

    int** parentPopulation;
    int** childPopulation;
    localSearch* LS;

    void randomRoute(int*);
    static void deleteSegmentOfArray(int**, int, int);
    std::pair<int*,int> getBestRoute();
    void selectChildrenForParents(int);
    void repairParents();
    void crossoverOperator(int );
    void randomMutateChildren(int );
    int* getCACO();

public:
    GeneticAlgorithm(int,int,int,int,int);
    virtual ~GeneticAlgorithm();
    void generateStartingPopulation(int,int);
    void runGenerations(int,int,int);
    void displayPopulation();
    void checkSolution();
};


#endif //TESTSAMPLECODE_GENETICALGORITHM_H
