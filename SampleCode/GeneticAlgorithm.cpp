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

    parentPopulation = new int *[sizeOfPopulation];
    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex) {
        parentPopulation[populationIndex] = new int[NUM_OF_CUSTOMERS];
        for (int customers = 0; customers < NUM_OF_CUSTOMERS; ++customers)
            parentPopulation[populationIndex][customers] = -1;
    }
}

/*
 * Genetic Algorithm Destructor.
 */
GeneticAlgorithm::~GeneticAlgorithm() {
    for (int i = 0; i < sizeOfPopulation; ++i)
        delete[] parentPopulation[i];
    delete[] parentPopulation;
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
    for (i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        object = (int) ((rand() / (RAND_MAX + 1.0)) * (double) (NUM_OF_CUSTOMERS - tot_assigned));
        help = route[i];
        route[i] = route[i + object];
        route[i + object] = help;
        tot_assigned++;
    }

    //THIS IS WHERE LK-SEARCH WOULD BE CALLED.
    //LS->twoOptLocalSearch(route);
}

void GeneticAlgorithm::generateStartingPopulation() {
    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex)
        randomRoute(parentPopulation[populationIndex]);
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

void GeneticAlgorithm::runGenerations() {

}
