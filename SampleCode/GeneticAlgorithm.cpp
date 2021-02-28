#include <climits>
#include <algorithm>
#include "GeneticAlgorithm.h"

/*
 * ================================================================================ *
 * GENETIC ALGORITHM
 * ================================================================================ *
 */

/*
 * Genetic Algorithm Constructor.
 */
GeneticAlgorithm::GeneticAlgorithm(int SizeOfPopulation, int Generations) {
    LS = new localSearch(3, 3);

    sizeOfPopulation = SizeOfPopulation;
    generations = Generations;
    childPopulationCounter = 0;

    parentPopulation = new int *[sizeOfPopulation];
    childPopulation = new int *[sizeOfPopulation+sizeOfPopulation];
    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex) {
        parentPopulation[populationIndex] = new int[NUM_OF_CUSTOMERS];
        for (int customers = 0; customers < NUM_OF_CUSTOMERS; ++customers)
            parentPopulation[populationIndex][customers] = -1;
    }

    for (int populationIndex = 0; populationIndex < sizeOfPopulation+sizeOfPopulation; ++populationIndex) {
        childPopulation[populationIndex] = new int[NUM_OF_CUSTOMERS];
        for (int customers = 0; customers < NUM_OF_CUSTOMERS; ++customers)
            childPopulation[populationIndex][customers] = -1;
    }
}

/*
 * Genetic Algorithm Destructor.
 */
GeneticAlgorithm::~GeneticAlgorithm() {
    deleteSegmentOfArray(parentPopulation,0,sizeOfPopulation);
    delete[] parentPopulation;
    delete[] childPopulation;
    delete LS;
}

void GeneticAlgorithm::randomRoute(int *route) {
    //generate a random solution for the random heuristic
    int i, help, object, tot_assigned = 0;

//    route = new int[NUM_OF_CUSTOMERS + 1];
    //set indexes of objects.
    for (i = 1; i <= NUM_OF_CUSTOMERS; i++) {
        route[i - 1] = i;
    }
    //randomly change indexes of objects.
    for (i = 0; i < NUM_OF_CUSTOMERS; i++) {
        object = (int) ((rand() / (RAND_MAX + 1.0)) * (double) (NUM_OF_CUSTOMERS - tot_assigned));
        help = route[i];
        route[i] = route[i + object];
        route[i + object] = help;
        tot_assigned++;
    }
    route[NUM_OF_CUSTOMERS] = 0;
    //THIS IS WHERE LK-SEARCH WOULD BE CALLED.
    LS->randomPheromoneLocalSearchWithTwoOpt(route);
}

void GeneticAlgorithm::generateStartingPopulation() {
//    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex)
//        randomRoute(parentPopulation[populationIndex]);

    //DEBUGGING
//    displayPopulation();

    //DEBUGGING - generates random children.
    for (int populationIndex = 0; populationIndex < sizeOfPopulation+sizeOfPopulation; ++populationIndex) {
        randomRoute(childPopulation[populationIndex]);
        childPopulationCounter++;
    }
    //DEBUGGING - selects optimal children to replace parents.
    selectChildrenForParents();
}

/*
 * Displays the current populations.
 */
void GeneticAlgorithm::displayPopulation() {
    printf("-- Parent Population --\n");
    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex) {
        printf("Population %d:\n", populationIndex + 1);
        for (int customer = 0; customer < NUM_OF_CUSTOMERS; ++customer)
            printf("%d, ", parentPopulation[populationIndex][customer]);
        printf("\n");
    }

//    printf("-- Child Population --\n");
//    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex) {
//        printf("Population %d:\n", populationIndex + 1);
//        for (int customer = 0; customer < NUM_OF_CUSTOMERS; ++customer)
//            printf("%d, ", childPopulation[populationIndex][customer]);
//        printf("\n");
//    }
}

void GeneticAlgorithm::deleteSegmentOfArray(int** population, int begin, int end){
    for (int popCounter = begin; popCounter < end; ++popCounter)
        delete[] population[popCounter];
}

std::pair<int*,int> GeneticAlgorithm::getBestRoute(){
    int* CB, CBCount;
    double CBLength = INT_MAX,CLength;
    for (int popCounter = 0; popCounter < sizeOfPopulation; ++popCounter) {
         CLength = localSearch::getBasicLength(parentPopulation[popCounter]);
         if(CBLength > CLength){
             CBLength = CLength;
             CB = parentPopulation[popCounter];
             CBCount = popCounter;
         }
    }
    return {CB,CBCount};
}

//Add mutations after selection.
void GeneticAlgorithm::runGenerations() {
    for (int x = 1; x <= generations; ++x) {
        childPopulationCounter = 0;
        partitionCrossoverOperator();
        selectChildrenForParents();
    }
}

void GeneticAlgorithm::partitionCrossoverOperator(){
//    std::pair<int*,int> currentBest = getBestRoute();
    for (int recombineCounter = 1; recombineCounter < sizeOfPopulation; ++recombineCounter)
            PCRecombine(parentPopulation[0], parentPopulation[recombineCounter]);
}

void GeneticAlgorithm::PCRecombine(int* currentBest, int* toCombine){
    /*
     * Recombination of routes,
     * add recombinations to child population.
     */
    
}

bool sortCriteria(std::pair<int*, double> A,std::pair<int*, double> B){
    return (A.second > B.second);
}

void GeneticAlgorithm::selectChildrenForParents(){
    auto children = new std::vector<std::pair<int*, double>>;
    for (int popCounter = 0; popCounter < childPopulationCounter; ++popCounter)
        children->push_back({childPopulation[popCounter],localSearch::getBasicLength(childPopulation[popCounter])});
    std::sort(children->begin(),children->end(),sortCriteria);

    for (auto & i : *children) {
        printf("%f, ",i.second);
    }printf("\n");

    deleteSegmentOfArray(parentPopulation,0,sizeOfPopulation);

    for (int popCounter = 0; popCounter < sizeOfPopulation; ++popCounter) {
        parentPopulation[popCounter] = children->back().first;
        children->pop_back();
    }

    for (auto & i : *children) {
        printf("%f, ",i.second);
    }printf("\n");

    for (auto & i : *children) {
        delete[] children->back().first;
        children->pop_back();
    }
    childPopulationCounter = 0;

}
