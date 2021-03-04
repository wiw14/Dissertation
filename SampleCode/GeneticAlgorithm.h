//
// Created by wmw13 on 19/02/2021.
//

#ifndef TESTSAMPLECODE_GENETICALGORITHM_H
#define TESTSAMPLECODE_GENETICALGORITHM_H

#include "LocalSearches.h"
#include <climits>
#include <algorithm>
#include <deque>
#include <list>

class GeneticAlgorithm {
private:
    int sizeOfPopulation,childPopulationCounter;
    int generations;
    int** parentPopulation;
    int** childPopulation;
    localSearch* LS;

    void randomRoute(int*);
    static void deleteSegmentOfArray(int**, int, int);
    std::pair<int*,int> getBestRoute();
    void selectChildrenForParents();

    //Testing
    void testRecombination(int*, int*);

    void partitionCrossoverOperator();
    void PCRecombine(int*, int*);
    static std::map<int, std::pair<int, std::string> *>* createEdgeTable(int*,int*);
    static void displayEdgeTable(std::map<int, std::pair<int, std::string> *>*);
    static std::list<int>* createDegreeList(std::map<int, std::pair<int, std::string> *>*);
    std::list<std::list<int>*>* createPartitions(std::map<int, std::pair<int, std::string> *>*, std::list<int>* );
    static bool checkIfCustomerVisited(std::list<int>*, int);
    static void displayList(std::list<int>*);

public:
    GeneticAlgorithm(int,int);
    virtual ~GeneticAlgorithm();
    void generateStartingPopulation();
    void runGenerations();
    void displayPopulation();
    void checkSolution();
};


#endif //TESTSAMPLECODE_GENETICALGORITHM_H
