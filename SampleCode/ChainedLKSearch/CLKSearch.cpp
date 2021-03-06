//
// Created by wmw13 on 06/03/2021.
//

#include "CLKSearch.h"

CLKSearch::CLKSearch(){
    route = new int[NUM_OF_CUSTOMERS+1];
    createInitialRoute();
    displayRoute();
}

CLKSearch::~CLKSearch() {
    delete[] route;
}

void CLKSearch::displayRoute(){
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        printf("%d, ",route[i]);
    printf("\n");
}


/*
 * INITIAL ROUTE GENERATION.
 */
void CLKSearch::generaterRandomTour(){
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
}

void CLKSearch::createInitialRoute() {
    generaterRandomTour();
}

/*
 * DOUBLE BRIDGE OPERATOR.
 */
