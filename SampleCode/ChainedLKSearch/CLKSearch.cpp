#include "CLKSearch.h"

/*
 * Constructor, initialises route.
 * Calls the needed function for CLKSearch to operate.
 */
CLKSearch::CLKSearch(){
    route = new int[NUM_OF_CUSTOMERS+1];
    createInitialRoute();
    displayRoute();
}

/*
 * Frees memory used for the route.
 */
CLKSearch::~CLKSearch() {
    delete[] route;
}

/*
 * Displays the current route.
 * Iterates through all the customers in order.
 */
void CLKSearch::displayRoute(){
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        printf("%d, ",route[i]);
    printf("\n");
}


/*
 * INITIAL ROUTE GENERATION.
 */
/*
 * Generates a random route.
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

/*
 * Creates the initial route using a route generator.
 */
void CLKSearch::createInitialRoute() {
    generaterRandomTour();
}

/*
 * DOUBLE BRIDGE OPERATOR.
 */
