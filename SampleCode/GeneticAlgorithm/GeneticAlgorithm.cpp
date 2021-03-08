#include "GeneticAlgorithm.h"

/*
 * ================================================================================ *
 * GENETIC ALGORITHM.
 * ================================================================================ *
 */
#include "../ClusterBasedACO/CACO.h"

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
//    LS->randomPheromoneLocalSearchWithTwoOpt(parentPopulation[0]);
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

int* GeneticAlgorithm::getCACO(){
    //        printf("Generating Parent %d\n",i);
    srand (rand());
    KMeansClustering::createClusters(4);
    int numAnts= 3, iterations = 5, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.9, Q = 1,alpha = 0.6, beta=2.1;
    auto* a = new CACO(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta,twoOptIteration,randomSearchIteration);

    a->optimize(iterations);
//        printf("End Generating Parent %d\n",i);

    int* temp = a->returnResults();

//    LS->twoOptLocalPheromoneAddonSearch(temp); //BEST
//    LS->randomPheromoneLocalSearchWithTwoOpt(temp);
//    LS->LKSearch(temp);
    delete a;
    KMeansClustering::freeClusters();
    return temp;
}

/*
 * Creates a random population of children which are then selected to be parents.
 */
void GeneticAlgorithm::generateStartingPopulation() {
//    for (int populationIndex = 0; populationIndex < sizeOfPopulation + sizeOfPopulation; ++populationIndex) {
//        randomRoute(childPopulation[populationIndex]);
//        childPopulationCounter++;
//    }
//    selectChildrenForParents();

    for (int i = 0; i < sizeOfPopulation; ++i) {
        parentPopulation[i] = getCACO();
    }
//    printf("Finished Start POP\n");

//    parentPopulation[0] = new int[NUM_OF_CUSTOMERS+1]{0, 31, 14, 17, 15, 1, 13, 11, 2, 30, 26, 29, 4, 5, 6, 7, 8, 9, 10, 32, 12, 3, 18, 19, 21, 22, 20, 23, 24, 16, 28, 27, 25};
//    parentPopulation[1] = new int[NUM_OF_CUSTOMERS+1]{0, 31, 17, 25, 24, 23, 20, 22, 21, 19, 18, 4, 5, 6, 7, 8, 9, 10, 32, 3, 29, 28, 16, 27, 26, 15, 13, 30, 14, 1, 11, 12, 2};
//    parentPopulation[2] = new int[NUM_OF_CUSTOMERS+1]{0, 29, 28, 16, 27, 26, 17, 15, 25, 24, 23, 20, 22, 21, 19, 18, 13, 12, 4, 30, 14, 1, 31, 3, 5, 6, 7, 8, 9, 10, 32, 11, 2};
//    parentPopulation[3] = new int[NUM_OF_CUSTOMERS+1]{0, 3, 5, 6, 32, 10, 9, 8, 7, 4, 1, 25, 23, 20, 22, 21, 19, 18, 11, 12, 2, 29, 28, 16, 27, 26, 24, 13, 30, 17, 15, 14, 31};
//    parentPopulation[4] = new int[NUM_OF_CUSTOMERS+1]{0, 1, 14, 31, 30, 13, 18, 19, 22, 21, 20, 23, 24, 27, 16, 28, 29, 15, 26, 25, 17, 10, 12, 3, 4, 5, 6, 7, 9, 8, 32, 11, 2};
//    parentPopulation[5] = new int[NUM_OF_CUSTOMERS+1]{0, 30, 31, 14, 11, 12, 2, 1, 15, 17, 25, 24, 23, 20, 22, 21, 19, 18, 13, 4, 7, 6, 8, 9, 10, 32, 5, 3, 29, 28, 16, 27, 26};
//    parentPopulation[6] = new int[NUM_OF_CUSTOMERS+1]{0, 30, 31, 14, 1, 3, 2, 12, 11, 32, 9, 7, 5, 4, 13, 25, 24, 23, 20, 22, 21, 19, 18, 10, 8, 6, 15, 17, 26, 27, 16, 28, 29};
//    parentPopulation[7] = new int[NUM_OF_CUSTOMERS+1]{0, 4, 5, 6, 7, 8, 9, 10, 32, 3, 1, 14, 15, 17, 24, 23, 20, 22, 21, 19, 18, 13, 26, 25, 11, 12, 2, 30, 29, 28, 16, 27, 31};
//    parentPopulation[8] = new int[NUM_OF_CUSTOMERS+1]{0, 13, 25, 26, 27, 16, 29, 31, 15, 17, 19, 18, 21, 22, 20, 23, 24, 28, 30, 14, 1, 11, 12, 3, 4, 5, 6, 7, 8, 9, 10, 32, 2};
//    parentPopulation[9] = new int[NUM_OF_CUSTOMERS+1]{0, 3, 2, 12, 11, 32, 10, 8, 9, 4, 13, 18, 19, 21, 22, 20, 23, 24, 25, 17, 15, 1, 30, 31, 14, 6, 7, 5, 26, 27, 16, 28, 29};

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
//        printf("GENERATION %d\n",x);
        childPopulationCounter = 0;
//        printf("Before Crossover\n");
        crossoverOperator();
//        randomMutateChildren();
//        printf("Before Selection\n");
        selectChildrenForParents();
//        printf("END GENERATION\n");
        //repairParents();
    }
}

/*
 * Calls the different crossover operators to generate children from the parents.
 */
void GeneticAlgorithm::crossoverOperator() {
    for (int recombineCounter = 1; recombineCounter < sizeOfPopulation; ++recombineCounter) {
        int** tempChildren = CrossoverOperators::PCRecombine(parentPopulation[0], parentPopulation[recombineCounter]);
//      int** tempChildren = CrossoverOperators::testRecombination(parentPopulation[0],parentPopulation[recombineCounter]);
//        int** tempChildren = CrossoverOperators::partiallyMappedCrossover(parentPopulation[0],parentPopulation[recombineCounter]);

        //Add the generated children to the children population.
        childPopulation[childPopulationCounter++] = tempChildren[0];
        if(tempChildren[1][0] != INT_MAX)
            childPopulation[childPopulationCounter++] = tempChildren[1];
    }
    if(childPopulationCounter < sizeOfPopulation) {
//        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//            childPopulation[childPopulationCounter][i] = parentPopulation[0][i];
        childPopulation[childPopulationCounter++] = getCACO();
//        printf("CONTINGENCY\n");
    }
}

/*
 * Uses a selection operator to select viable children to be parents.
 */
void GeneticAlgorithm::selectChildrenForParents() {
    deleteSegmentOfArray(parentPopulation, 0, sizeOfPopulation);
//    printf("BEFORE SELECTION\n");
//    for (int i = 0; i < childPopulationCounter; ++i) {
//        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
//            printf("%d, ",childPopulation[i][j]);
//        }printf("\n");
//    }
    parentPopulation = Selection::greedySelection(childPopulation,childPopulationCounter,sizeOfPopulation);
    childPopulationCounter = 0;
//    printf("END SELECTION\n");
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
