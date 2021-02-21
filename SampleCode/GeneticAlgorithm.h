//
// Created by wmw13 on 19/02/2021.
//

#ifndef TESTSAMPLECODE_GENETICALGORITHM_H
#define TESTSAMPLECODE_GENETICALGORITHM_H

#include "LocalSearches.h"

class GeneticAlgorithm {
private:
    int sizeOfPopulation;
    int generations;
    int** parentPopulation;
    int** childPopulation;
    localSearch* LS;

    void randomRoute(int*);

public:
    GeneticAlgorithm(int,int);
    virtual ~GeneticAlgorithm();
    void generateStartingPopulation();
    void runGenerations();
    void displayPopulation();
};


#endif //TESTSAMPLECODE_GENETICALGORITHM_H
