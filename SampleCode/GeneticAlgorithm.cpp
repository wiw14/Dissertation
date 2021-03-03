#include <climits>
#include <algorithm>
#include <deque>
#include <list>
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
    childPopulation = new int *[sizeOfPopulation + sizeOfPopulation];
    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex) {
        parentPopulation[populationIndex] = new int[NUM_OF_CUSTOMERS + 1];
        for (int customers = 0; customers <= NUM_OF_CUSTOMERS; ++customers)
            parentPopulation[populationIndex][customers] = -1;
    }

    for (int populationIndex = 0; populationIndex < sizeOfPopulation + sizeOfPopulation; ++populationIndex) {
        childPopulation[populationIndex] = new int[NUM_OF_CUSTOMERS + 1];
        for (int customers = 0; customers <= NUM_OF_CUSTOMERS; ++customers)
            childPopulation[populationIndex][customers] = -1;
    }
}

/*
 * Genetic Algorithm Destructor.
 */
GeneticAlgorithm::~GeneticAlgorithm() {
    deleteSegmentOfArray(parentPopulation, 0, sizeOfPopulation);
    delete[] parentPopulation;
    delete[] childPopulation;
    delete LS;
}

void GeneticAlgorithm::checkSolution() {
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
    // LS->LKSearch(route);
}

void GeneticAlgorithm::generateStartingPopulation() {
    //DEBUGGING - generates random children.
    for (int populationIndex = 0; populationIndex < sizeOfPopulation + sizeOfPopulation; ++populationIndex) {
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

void GeneticAlgorithm::deleteSegmentOfArray(int **population, int begin, int end) {
    for (int popCounter = begin; popCounter < end; ++popCounter)
        delete[] population[popCounter];
}

std::pair<int *, int> GeneticAlgorithm::getBestRoute() {
    int *CB, CBCount;
    double CBLength = INT_MAX, CLength;
    for (int popCounter = 0; popCounter < sizeOfPopulation; ++popCounter) {
        CLength = localSearch::getBasicLength(parentPopulation[popCounter]);
        if (CBLength > CLength) {
            CBLength = CLength;
            CB = parentPopulation[popCounter];
            CBCount = popCounter;
        }
    }
    return {CB, CBCount};
}

//Add mutations after selection.
void GeneticAlgorithm::runGenerations() {
    for (int x = 1; x <= generations; ++x) {
        childPopulationCounter = 0;
        partitionCrossoverOperator();
        selectChildrenForParents();
    }
}

void GeneticAlgorithm::partitionCrossoverOperator() {
    for (int recombineCounter = 1; recombineCounter < sizeOfPopulation; ++recombineCounter) {
        PCRecombine(parentPopulation[0], parentPopulation[recombineCounter]);
//        testRecombination(parentPopulation[0],parentPopulation[recombineCounter]);
    }
}

void GeneticAlgorithm::testRecombination(int *currentBest, int *toCombine) {
    int *childOne = new int[NUM_OF_CUSTOMERS + 1], *childTwo = new int[NUM_OF_CUSTOMERS + 1];
    bool *visitedOne = new bool[NUM_OF_CUSTOMERS + 1], *visitedTwo = new bool[NUM_OF_CUSTOMERS + 1];

    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        visitedOne[i] = false;
        visitedTwo[i] = false;
    }

    bool SWITCH = false;
    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index) {
        if (currentBest[index] == toCombine[index]) {
            SWITCH = !SWITCH;
            while (currentBest[index] == toCombine[index] && index <= NUM_OF_CUSTOMERS) {
                childOne[index] = currentBest[index];
                visitedOne[childOne[index]] = true;
                childTwo[index] = currentBest[index];
                visitedTwo[childTwo[index]] = true;
                index++;
            }
            index--;
        } else {
            if (SWITCH && !visitedOne[currentBest[index]] && !visitedTwo[toCombine[index]]) {
                childOne[index] = currentBest[index];
                visitedOne[childOne[index]] = true;
                childTwo[index] = toCombine[index];
                visitedTwo[childTwo[index]] = true;
            } else {
                if (!visitedOne[toCombine[index]] && !visitedTwo[currentBest[index]]) {
                    childOne[index] = toCombine[index];
                    visitedOne[childOne[index]] = true;
                    childTwo[index] = currentBest[index];
                    visitedTwo[childTwo[index]] = true;
                } else {
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
    childPopulation[childPopulationCounter++] = childOne;
    childPopulation[childPopulationCounter++] = childTwo;
}

void GeneticAlgorithm::PCRecombine(int *currentBest, int *toCombine) {
    /*
     * Recombination of routes,
     * add recombinations to child population.
     */
    auto edgeTable = new std::map<int, std::pair<int, std::string> *>();
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        auto *tempArray = new std::pair<int, std::string>[4];
        for (int j = 0; j < 4; ++j) {
            tempArray[j].first = INT_MAX;
            tempArray[j].second = "";
        }
        edgeTable->emplace(i, tempArray);
    }

    //DEBUGGING
    printf("Current Best : ");
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        printf("%d, ", currentBest[i]);
    printf("\n");
    printf("To Combine : ");
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        printf("%d, ", toCombine[i]);
    printf("\n");
    /*
     * STEP 1:
     * Create edge table.
     */
    //Lambda function for adding vertices to edge array.
    auto addToArray = [&](std::pair<int, std::string> *array, const int *currentArray, int index) {

        for (int arrayIndex = 0; arrayIndex < 4; ++arrayIndex) {
            if (array[arrayIndex].first == INT_MAX) {
                array[arrayIndex].first = currentArray[index];
                break;
            } else if (array[arrayIndex].first == currentArray[index]) {
                array[arrayIndex].second = "'";
                break;
            }
        }
    };

    //Loop through all the customers in both routes and generate an edge table.
    for (int customerIndex = 0; customerIndex <= NUM_OF_CUSTOMERS; ++customerIndex) {
        std::pair<int, std::string> *bestArray = edgeTable->at(currentBest[customerIndex]);
        std::pair<int, std::string> *toCombineArray = edgeTable->at(toCombine[customerIndex]);

        if (customerIndex != 0) {
            addToArray(bestArray, currentBest, customerIndex - 1);
            addToArray(toCombineArray, toCombine, customerIndex - 1);
            if (customerIndex == NUM_OF_CUSTOMERS) {
                addToArray(bestArray, currentBest, 0);
                addToArray(toCombineArray, toCombine, 0);
            }
        }
        if (customerIndex != NUM_OF_CUSTOMERS) {
            addToArray(bestArray, currentBest, customerIndex + 1);
            addToArray(toCombineArray, toCombine, customerIndex + 1);
            if (customerIndex == 0) {
                addToArray(bestArray, currentBest, NUM_OF_CUSTOMERS);
                addToArray(toCombineArray, toCombine, NUM_OF_CUSTOMERS);
            }
        }
    }

    //Displays edge table.
    //DEBUGGING
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
//        printf("%d : ", i);
//        std::pair<int, std::string> *tempArray = edgeTable->at(i);
//        for (int j = 0; j < 4; ++j) {
//            if (tempArray[j].first == INT_MAX)
//                break;
//            printf("%d%s, ", tempArray[j].first, tempArray[j].second.c_str());
//        }
//        printf("\n");
//    }
//    printf("-----------------------------\n");

    /*
    * STEP 2a:
     * Creates additional lists.
    */

    //Creates and initialises LIST1.
    int numDegreeTwo = 0;
    auto LIST1 = new std::list<int>(); //insert at both head and tail.
    for (int customerIndex = 0; customerIndex <= NUM_OF_CUSTOMERS; ++customerIndex) {
        int degree = 0;
        for (int arrayIndex = 0; arrayIndex < 4; ++arrayIndex) {
            if (edgeTable->at(customerIndex)[arrayIndex].first == INT_MAX)
                break;
            degree++;
        }
//        printf("%d=%d\n", customerIndex, degree);
        if (degree > 2) {
            LIST1->push_back(customerIndex);
        } else {
            LIST1->push_front(customerIndex);
            numDegreeTwo++;
        }
    }
    int cPointer = numDegreeTwo;

    //Creates and initialises LIST2, index list.
    auto indexList = new int[NUM_OF_CUSTOMERS + 1];
    //Indexing.
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        indexList[*std::next(LIST1->begin(), i)] = i;
    }
    int iPointer = NUM_OF_CUSTOMERS;

    //DEBUGGING
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        printf("%d:%d,", i, indexList[i]);
    printf("\n");
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        printf("%d:%d,", i, *std::next(LIST1->begin(), i));
    printf("\n");
    printf("%d:%d, %d:%d\n", iPointer, *std::next(LIST1->begin(), iPointer), cPointer,
           *std::next(LIST1->begin(), cPointer));

    /*
     * STEP 2b:
     * Generate partitions.
     */
    auto customerQueue = std::deque<int>();
    auto partitioned = new bool[NUM_OF_CUSTOMERS + 1];
    int partitionCounter = 0;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        partitioned[i] = false;
    while (!(iPointer == numDegreeTwo)) {
        cPointer = numDegreeTwo;
        customerQueue.push_front(*std::next(LIST1->begin(), cPointer));
        printf("%d\n", partitionCounter);
        printf("==%d:%d, %d:%d\n", iPointer, *std::next(LIST1->begin(), iPointer), cPointer,
               *std::next(LIST1->begin(), cPointer));
        partitionCounter++;
        while (!customerQueue.empty()) {
            int currentNode = customerQueue.back();
            partitioned[currentNode] = true;
            customerQueue.pop_back();
            cPointer = indexList[currentNode];
            printf("Current Node = %d\n", currentNode);
            if (iPointer < cPointer || iPointer == numDegreeTwo)
                break;
            for (int i = 0; i < 4; ++i) {
                if (edgeTable->at(currentNode)[i].first == INT_MAX)
                    break;
                else if (edgeTable->at(currentNode)[i].second != "'") {
                    edgeTable->at(currentNode)[i].second = std::to_string(partitionCounter);
                    if (!partitioned[edgeTable->at(currentNode)[i].first])
                        customerQueue.push_front(edgeTable->at(currentNode)[i].first);
                }
            }
            indexList[*std::next(LIST1->begin(), cPointer)] = iPointer;
            indexList[*std::next(LIST1->begin(), iPointer)] = cPointer;

            std::swap(*std::next(LIST1->begin(), cPointer), *std::next(LIST1->begin(), iPointer));
            printf("%d:%d, %d:%d\n", iPointer, *std::next(LIST1->begin(), iPointer), cPointer,
                   *std::next(LIST1->begin(), cPointer));

            for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
                printf("%d:%d,", i, *std::next(LIST1->begin(), i));
            printf("\n");
            iPointer--;
        }
    }

    //DEBUGGING
    printf("NUMBER OF PARTITIONS : %d\n", partitionCounter);
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        printf("%d : ", i);
        std::pair<int, std::string> *tempArray = edgeTable->at(i);
        for (int j = 0; j < 4; ++j) {
            if (tempArray[j].first == INT_MAX)
                break;
            printf("%d:%s, ", tempArray[j].first, tempArray[j].second.c_str());
        }
        printf("\n");
    }
    printf("-----------------------------\n");

    /*
     * STEP 3
     * Create children from partitions.
     */
    if (partitionCounter > 1) {
        //DEBUGGING
        printf("PX Success.\n");
        int *childOne = new int[NUM_OF_CUSTOMERS + 1], *childTwo = new int[NUM_OF_CUSTOMERS + 1];
        int partitionA = -1, partitionB = -1;
        if (partitionCounter == 2) {
            partitionA = 1;
            partitionB = 2;
        } else {
            //Find 2 largest partition
            int *partitionSizes = new int[2]{-1, -1};
            for (int partitionIndex = 1; partitionIndex <= partitionCounter; ++partitionIndex) {
                int partitionSize = 0;
                for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
                    std::string temp = "";
                    for (int j = 0; j < 4; ++j) {
                        if (edgeTable->at(i)[j].first == INT_MAX)
                            break;
                        else if (edgeTable->at(i)[j].second != "'") {
                            temp = edgeTable->at(i)[j].second;
                            break;
                        }
                    }
                    if (temp == std::to_string(partitionCounter)) {
                        partitionSize++;
                    }
                }
                if (partitionSizes[0] < partitionSize) {
                    partitionSizes[1] = partitionSizes[0];
                    partitionSizes[0] = partitionSize;
                    partitionB = partitionA;
                    partitionA = partitionIndex;
                } else if (partitionSizes[1] < partitionSize) {
                    partitionSizes[1] = partitionSize;
                    partitionB = partitionIndex;
                }
            }
            delete[] partitionSizes;
        }
        printf("A: %d, B: %d\n", partitionA, partitionB);

        std::string type = "";
        for (int i = 0; i <= NUM_OF_CUSTOMERS ; ++i) {
            std::pair<int, std::string> *edges = edgeTable->at(currentBest[i]);


            for (int j = 0; j < 4; ++j) {
                if(edges[j].first == INT_MAX)
                    break;
                if(edges[j].second != "'" && edges[j].second != ""){
                    type = edges[j].second;
                 break;}
            }
            if (type == std::to_string(partitionB))
                childOne[i] = toCombine[i];
            else
                childOne[i] = currentBest[i];
        }
        type = "";
        for (int i = 0; i <= NUM_OF_CUSTOMERS ; ++i) {
            std::pair<int, std::string> *edges = edgeTable->at(toCombine[i]);


            for (int j = 0; j < 4; ++j) {
                if(edges[j].first == INT_MAX)
                    break;
                if(edges[j].second != "'" && edges[j].second != ""){
                    type = edges[j].second;
                    break;}
            }
            if (type == std::to_string(partitionB))
                childTwo[i] = currentBest[i];
            else
                childTwo[i] = toCombine[i];
        }

        printf("\n");
        printf("child one: ");
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
            printf("%d, ",childOne[i]);
        printf("\n");
        printf("child two: ");
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
            printf("%d, ",childTwo[i]);
        printf("\n");

        childPopulation[childPopulationCounter++] = childOne;
        childPopulation[childPopulationCounter++] = childTwo;
    } else {
        //DEBUGGING
        printf("PX Failure.\n");
        int *child = new int[NUM_OF_CUSTOMERS + 1];
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
            child[i] = toCombine[i];
        //printf("-%d\n",childPopulationCounter);
        LS->randomPheromoneLocalSearchWithTwoOpt(child);
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
            printf("%d, ",child[i]);
        printf("\n");
        childPopulation[childPopulationCounter++] = child;
    }

    /*
     * STEP 4:
     * Delete the edge table from memory.
     */
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        delete[] edgeTable->at(i);
    delete edgeTable;
}

bool sortCriteria(std::pair<int *, double> A, std::pair<int *, double> B) {
    return (A.second > B.second);
}

void GeneticAlgorithm::selectChildrenForParents() {
    auto children = new std::vector<std::pair<int *, double>>;

    //DEBUGGING
    printf("Number Children = %d\n", childPopulationCounter);
    for (int i = 0; i < childPopulationCounter; ++i) {
        printf("childs %d\n", i);
        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
            printf("%d, ", childPopulation[i][j]);
        }
        printf("\n");
    }


    for (int popCounter = 0; popCounter < childPopulationCounter; ++popCounter)
        children->push_back({childPopulation[popCounter], localSearch::getBasicLength(childPopulation[popCounter])});

    std::sort(children->begin(), children->end(), sortCriteria);

    //DEBUGGING
//    for (auto & i : *children) {
//        printf("%f, ",i.second);
//    }printf("\n");

    deleteSegmentOfArray(parentPopulation, 0, sizeOfPopulation);

    for (int popCounter = 0; popCounter < sizeOfPopulation; ++popCounter) {
        parentPopulation[popCounter] = children->back().first;
        children->pop_back();
    }

    //DEBUGGING
//    for (auto & i : *children) {
//        printf("%f, ",i.second);
//    }printf("\n");


    for (auto &i : *children) {
        delete[] children->back().first;
        children->pop_back();
    }
    childPopulationCounter = 0;
}
