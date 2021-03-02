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
        parentPopulation[populationIndex] = new int[NUM_OF_CUSTOMERS+1];
        for (int customers = 0; customers <= NUM_OF_CUSTOMERS; ++customers)
            parentPopulation[populationIndex][customers] = -1;
    }

    for (int populationIndex = 0; populationIndex < sizeOfPopulation+sizeOfPopulation; ++populationIndex) {
        childPopulation[populationIndex] = new int[NUM_OF_CUSTOMERS+1];
        for (int customers = 0; customers <= NUM_OF_CUSTOMERS; ++customers)
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

void GeneticAlgorithm::checkSolution(){
//    LS->twoOptLocalPheromoneAddonSearch(parentPopulation[0]);
    LS->getRouteLength(parentPopulation[0]);
}

void GeneticAlgorithm::randomRoute(int *route) {
    auto tempRoute = std::vector<int>();
    //Creates list of customers.
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        tempRoute.push_back(i);

    int randIndex;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        randIndex = rand() % tempRoute.size();
        route[i] = tempRoute[randIndex];
        tempRoute.erase(tempRoute.begin() + randIndex);
    }

    //Local Search to create local optimums.
    LS->randomPheromoneLocalSearchWithTwoOpt(route);
}

void GeneticAlgorithm::generateStartingPopulation() {
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
    for (int recombineCounter = 1; recombineCounter < sizeOfPopulation; ++recombineCounter) {
            PCRecombine(parentPopulation[0], parentPopulation[recombineCounter]);
//        testRecombination(parentPopulation[0],parentPopulation[recombineCounter]);
    }
}

void GeneticAlgorithm::testRecombination(int* currentBest, int* toCombine){
    int * childOne = new int[NUM_OF_CUSTOMERS+1], *childTwo = new int[NUM_OF_CUSTOMERS+1];
    bool * visitedOne = new bool[NUM_OF_CUSTOMERS+1], *visitedTwo = new bool[NUM_OF_CUSTOMERS+1];

    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        visitedOne[i] = false;
        visitedTwo[i] = false;
    }

    bool SWITCH = false;
    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index) {
        if(currentBest[index] == toCombine[index]){
            SWITCH = !SWITCH;
            while(currentBest[index] == toCombine[index] && index <= NUM_OF_CUSTOMERS){
                childOne[index] = currentBest[index];
                visitedOne[childOne[index]] = true;
                childTwo[index] = currentBest[index];
                visitedTwo[childTwo[index]] = true;
                index++;
            } index--;
        }else {
            if (SWITCH && !visitedOne[currentBest[index]] && !visitedTwo[toCombine[index]]) {
                childOne[index] = currentBest[index];
                visitedOne[childOne[index]] = true;
                childTwo[index] = toCombine[index];
                visitedTwo[childTwo[index]] = true;
            } else {
                if(!visitedOne[toCombine[index]] && !visitedTwo[currentBest[index]]) {
                    childOne[index] = toCombine[index];
                    visitedOne[childOne[index]] = true;
                    childTwo[index] = currentBest[index];
                    visitedTwo[childTwo[index]] = true;
                }
                else{
                    childOne[index] = currentBest[index];
                    visitedOne[childOne[index]] = true;
                    childTwo[index] = toCombine[index];
                    visitedTwo[childTwo[index]] = true;
                }
            }
        }
    }
    delete[] visitedOne;
    delete[] visitedTwo;
    childPopulation[childPopulationCounter++] = childOne; childPopulation[childPopulationCounter++] = childTwo;
}

void GeneticAlgorithm::PCRecombine(int* currentBest, int* toCombine){
    /*
     * Recombination of routes,
     * add recombinations to child population.
     */
    auto edgeTable = new std::map<int, int*>();
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        int* tempArray = new int[4];
        for (int j = 0; j < 4; ++j)
           tempArray[j] = -1;
        edgeTable->emplace(i,tempArray);
    }

    printf("Current Best : ");
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        printf("%d, ",currentBest[i]);
    printf("\n");

    printf("To Combine : ");
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        printf("%d, ",toCombine[i]);
    printf("\n");

    /*
     * STEP 1:
     * Create edge table.
     */
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        if(i == 0){
            int* tempArray = edgeTable->at(currentBest[i]);
            for (int j = 0; j < 4; ++j) {
                if(tempArray[j] == -1){
                    tempArray[j] = currentBest[i+1];
                    tempArray[j+1] = currentBest[NUM_OF_CUSTOMERS];
                    break;
                }
                else if(tempArray[j] == currentBest[i+1] ){
                    tempArray[j] = tempArray[j]*-1;
                    for (int k = j; k < 4; ++k) {
                        if(tempArray[k] == -1){
                            tempArray[k] = currentBest[NUM_OF_CUSTOMERS];
                            break;
                        }
                        else if(tempArray[k] == currentBest[NUM_OF_CUSTOMERS]){
                            tempArray[k] = tempArray[k]*-1;
                            break;
                        }
                    }
                    break;
                }
            }
            tempArray = edgeTable->at(toCombine[i]);
            for (int j = 0; j < 4; ++j) {
                if(tempArray[j] == -1){
                    tempArray[j] = toCombine[i+1];
                    tempArray[j+1] = toCombine[NUM_OF_CUSTOMERS];
                    break;
                }
                else if(tempArray[j] == toCombine[i+1] ){
                    tempArray[j] = tempArray[j]*-1;
                    for (int k = j; k < 4; ++k) {
                        if(tempArray[k] == -1){
                            tempArray[k] = toCombine[NUM_OF_CUSTOMERS];
                            break;
                        }
                        else if(tempArray[k] == toCombine[NUM_OF_CUSTOMERS]){
                            tempArray[k] = tempArray[k]*-1;
                            break;
                        }
                    }
                    break;
                }
            }
        }else if (i == NUM_OF_CUSTOMERS){
            int* tempArray = edgeTable->at(currentBest[i]);
            for (int j = 0; j < 4; ++j) {
                if(tempArray[j] == -1){
                    tempArray[j] = currentBest[i-1];
                    break;
                }
                else if(tempArray[j] == currentBest[i-1] ){
                    tempArray[j] = tempArray[j]*-1;
                    break;
                }
            }
            tempArray = edgeTable->at(toCombine[i]);
            for (int j = 0; j < 4; ++j) {
                if(tempArray[j] == -1){
                    tempArray[j] = toCombine[i-1];
                    break;
                }
                else if(tempArray[j] == toCombine[i-1] ){
                    tempArray[j] = tempArray[j]*-1;
                    break;
                }
            }
        }else{
            int* tempArray = edgeTable->at(currentBest[i]);
            for (int j = 0; j < 4; ++j) {
                if(tempArray[j] == -1){
                    tempArray[j] = currentBest[i-1];
                    printf("-%d,%d::",currentBest[i-1],j);
                    for (int k = j; k < 4; ++k) {
                        if(tempArray[k] == -1){
                            tempArray[k] = currentBest[i+1];
                            printf("%d,%d\n",currentBest[i+1],k);
                            break;
                        }
                        else if(tempArray[k] == currentBest[i+1]){
                            tempArray[k] = tempArray[k]*-1;
                            break;
                        }
                    }
                    break;
                }
                else if(tempArray[j] == currentBest[i+1] ){
                    tempArray[j] = tempArray[j]*-1;
                    for (int k = j+1; k < 4; ++k) {
                        if(tempArray[k] == -1){
                            tempArray[k] = currentBest[i-1];
                            break;
                        }
                        else if(tempArray[k] == currentBest[i-1]){
                            tempArray[k] = tempArray[k]*-1;
                            break;
                        }
                    }
                    break;
               }
                else if(tempArray[j] == currentBest[i-1]){
                    tempArray[j] = tempArray[j]*-1;
                    for (int k = j+1; k < 4; ++k) {
                        if(tempArray[k] == -1){
                            tempArray[k] = currentBest[i+1];
                            break;
                        }
                        else if(tempArray[k] == currentBest[i+1]){
                            tempArray[k] = tempArray[k]*-1;
                            break;
                        }
                    }
                    break;
                }
            }
            tempArray = edgeTable->at(toCombine[i]);
            for (int j = 0; j < 4; ++j) {
                if(tempArray[j] == -1){
                    tempArray[j] = toCombine[i-1];
                    for (int k = j; k < 4; ++k) {
                        if(tempArray[k] == -1){
                            tempArray[k] = toCombine[i+1];
                            break;
                        }
                        else if(tempArray[k] == toCombine[i+1]){
                            tempArray[k] = tempArray[k]*-1;
                            break;
                        }
                    }
                    break;
                }
                else if(tempArray[j] == toCombine[i+1] ){
                    tempArray[j] = tempArray[j]*-1;
                    for (int k = j; k < 4; ++k) {
                        if(tempArray[k] == -1){
                            tempArray[k] = toCombine[i-1];
                            break;
                        }
                        else if(tempArray[k] == toCombine[i-1]){
                            tempArray[k] = tempArray[k]*-1;
                            break;
                        }
                    }
                    break;
                }
                else if(tempArray[j] == toCombine[i-1]){
                    tempArray[j] = tempArray[j]*-1;
                    for (int k = j; k < 4; ++k) {
                        if(tempArray[k] == -1){
                            tempArray[k] = toCombine[i+1];
                            break;
                        }
                        else if(tempArray[k] == toCombine[i+1]){
                            tempArray[k] = tempArray[k]*-1;
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    //Displays edge table.
    //DEBUGGING
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        printf("%d : ",i);
        int* tempArray = edgeTable->at(i);
        for (int j = 0; j < 4; ++j) {
            if(tempArray[j] == -1)
                break;
            printf("%d, ",tempArray[j]);
        }
        printf("\n");
    }
    printf("-----------------------------\n");
     /*
     * STEP 2:
     * Remove edges with degree 2.
     */

     /*
     * STEP 3:
     *
     */
}

bool sortCriteria(std::pair<int*, double> A,std::pair<int*, double> B){
    return (A.second > B.second);
}

void GeneticAlgorithm::selectChildrenForParents(){
    auto children = new std::vector<std::pair<int*, double>>;

    //DEBUGGING
//    for (int i = 0; i <childPopulationCounter; ++i) {
//        printf("childs %d\n",i);
//        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
//            printf("%d, ",childPopulation[i][j]);
//        }printf("\n");
//    }


    for (int popCounter = 0; popCounter < childPopulationCounter; ++popCounter)
        children->push_back({childPopulation[popCounter],localSearch::getBasicLength(childPopulation[popCounter])});

    std::sort(children->begin(),children->end(),sortCriteria);

    //DEBUGGING
//    for (auto & i : *children) {
//        printf("%f, ",i.second);
//    }printf("\n");

    deleteSegmentOfArray(parentPopulation,0,sizeOfPopulation);

    for (int popCounter = 0; popCounter < sizeOfPopulation; ++popCounter) {
        parentPopulation[popCounter] = children->back().first;
        children->pop_back();
    }

    //DEBUGGING
//    for (auto & i : *children) {
//        printf("%f, ",i.second);
//    }printf("\n");

    for (auto & i : *children) {
        delete[] children->back().first;
        children->pop_back();
    }
    childPopulationCounter = 0;

}
