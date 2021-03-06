#include "Selection.h"

/*
 * Sorts the routes based upon their route length.
 */
bool sortCriteria(std::pair<int *, double> A, std::pair<int *, double> B) {
    return (A.second > B.second);
}

/*
 * Selects the best x number of children based upon route length.
 */
int** Selection::greedySelection(int** childPopulation, int childPopulationCounter,int sizeOfPopulation) {
    auto children = new std::vector<std::pair<int *, double>>;
    int** parentPopulation = new int*[sizeOfPopulation];
    //DEBUGGING
//    printf("Number Children = %d\n", childPopulationCounter);
//    for (int i = 0; i < childPopulationCounter; ++i) {
//        printf("childs %d\n", i);
//        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
//            printf("%d, ", childPopulation[i][j]);
//        }
//        printf("\n");
//    }


    for (int popCounter = 0; popCounter < childPopulationCounter; ++popCounter)
        children->push_back({childPopulation[popCounter], GenerateTour::getBasicLength(childPopulation[popCounter])});

    std::sort(children->begin(), children->end(), sortCriteria);

    //DEBUGGING
//    for (auto & i : *children) {
//        printf("%f, ",i.second);
//    }printf("\n");


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
//    *childPopulationCounter = 0;

    return parentPopulation;
}