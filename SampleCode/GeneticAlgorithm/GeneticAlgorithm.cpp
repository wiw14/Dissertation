#include "GeneticAlgorithm.h"

/*
 * ================================================================================ *
 * GENETIC ALGORITHM.
 * ================================================================================ *
 */

/*
 * Genetic Algorithm Constructor.
 */
GeneticAlgorithm::GeneticAlgorithm(int SizeOfPopulation, int Generations, int NumMutations) {
    LS = new localSearch(3, 3);

    sizeOfPopulation = SizeOfPopulation;
    generations = Generations;
    probabiltyOfMutation = NumMutations;
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

/*
 *
 */
void GeneticAlgorithm::checkSolution() {
//    LS->twoOptLocalPheromoneAddonSearch(parentPopulation[0]);
    GenerateTour::getRouteLength(parentPopulation[0]);
}

/*
 * Generates a random route of all the customers.
 */
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
//     LS->LKSearch(route);
}

/*
 * Creates a random population of children which are then selected to be parents.
 */
void GeneticAlgorithm::generateStartingPopulation() {
    for (int populationIndex = 0; populationIndex < sizeOfPopulation + sizeOfPopulation; ++populationIndex) {
        randomRoute(childPopulation[populationIndex]);
        childPopulationCounter++;
    }
    selectChildrenForParents();
}

/*
 * Displays the current populations.
 */
void GeneticAlgorithm::displayPopulation() {
    printf("-- Parent Population --\n");
    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex) {
        printf("Population %d:\n", populationIndex + 1);
        for (int customer = 0; customer <= NUM_OF_CUSTOMERS; ++customer)
            printf("%d, ", parentPopulation[populationIndex][customer]);
        printf("\n");
    }

}

/*
 * Frees routes from a population.
 */
void GeneticAlgorithm::deleteSegmentOfArray(int **population, int begin, int end) {
    for (int popCounter = begin; popCounter < end; ++popCounter)
        delete[] population[popCounter];
}

/*
 * Determines the best route within the parent population.
 */
std::pair<int *, int> GeneticAlgorithm::getBestRoute() {
    int *CB, CBCount;
    double CBLength = INT_MAX, CLength;
    for (int popCounter = 0; popCounter < sizeOfPopulation; ++popCounter) {
        CLength = GenerateTour::getBasicLength(parentPopulation[popCounter]);
        if (CBLength > CLength) {
            CBLength = CLength;
            CB = parentPopulation[popCounter];
            CBCount = popCounter;
        }
    }
    return {CB, CBCount};
}

/*
 * Main body of the genetic algorithm, calls the Crossover, mutation, selection and repair operators.
 */
void GeneticAlgorithm::runGenerations() {
    for (int x = 1; x <= generations; ++x) {
        childPopulationCounter = 0;
        crossoverOperator();
        randomMutateChildren();
        selectChildrenForParents();
        //repairParents();
    }
}

/*
 * Calls the different crossover operators to generate children from the parents.
 */
void GeneticAlgorithm::crossoverOperator() {
    for (int recombineCounter = 1; recombineCounter < sizeOfPopulation; ++recombineCounter) {
//        int** tempChildren = CrossoverOperators::PCRecombine(parentPopulation[0], parentPopulation[recombineCounter]);
//      int** tempChildren = CrossoverOperators::testRecombination(parentPopulation[0],parentPopulation[recombineCounter]);
        int** tempChildren = CrossoverOperators::partiallyMappedCrossover(parentPopulation[0],parentPopulation[recombineCounter]);

        //Add the generated children to the children population.
        childPopulation[childPopulationCounter++] = tempChildren[0];
        if(tempChildren[1][0] != INT_MAX)
            childPopulation[childPopulationCounter++] = tempChildren[1];
    }
    //childPopulation[childPopulationCounter++] = parentPopulation[0];
}

/*
 * Uses a selection operator to select viable children to be parents.
 */
void GeneticAlgorithm::selectChildrenForParents() {
    deleteSegmentOfArray(parentPopulation, 0, sizeOfPopulation);
    parentPopulation = Selection::greedySelection(childPopulation,childPopulationCounter,sizeOfPopulation);
    childPopulationCounter = 0;
}

/*
 * Uses a repair operator to repair issues (e.g. duplicate customers) within the parent routes.
 */
void GeneticAlgorithm::repairParents(){
    for (int i = 0; i < sizeOfPopulation; ++i)
        RepairOperators::basicRepair(parentPopulation[i]);
}

/*
 * Uses a mutation operator to mutate the children population based upon a predetermined probability.
 * This helps to reduce the Genetic Algorithm from getting stuck in local optimums.
 */
void GeneticAlgorithm::randomMutateChildren(){
    for (int i = 0; i < childPopulationCounter; ++i) {
        int toMutate = rand()%probabiltyOfMutation;
        if(toMutate == 0) {
            //printf("Mutate\n");
            //Mutation::randomSwapMutation(childPopulation[i]);
            Mutation::LKMutation(childPopulation[i],LS);
        }
    }
}
