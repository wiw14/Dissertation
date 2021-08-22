#include "GeneticAlgorithm.h"
#include "../Framework/stats.hpp"

/*
 * ================================================================================ *
 * GENETIC ALGORITHM.
 * ================================================================================ *
 */
#include "../ExternalClusteringACO/CACO.h"

/*
 * Genetic Algorithm Constructor.
 */
GeneticAlgorithm::GeneticAlgorithm(int SizeOfPopulation, int Generations, int NumMutations,int randomSearchIterations,int twoOptIterations) {
    LS = new localSearch(randomSearchIterations, twoOptIterations);

    sizeOfPopulation = SizeOfPopulation;
    generations = Generations;
    probabiltyOfMutation = NumMutations;
    childPopulationCounter = 0;

    parentPopulation = new int *[sizeOfPopulation];
    childPopulation = new int *[sizeOfPopulation + sizeOfPopulation];
    for (int populationIndex = 0; populationIndex < sizeOfPopulation; ++populationIndex) {
        parentPopulation[populationIndex] = nullptr;
    }

    for (int populationIndex = 0; populationIndex < sizeOfPopulation + sizeOfPopulation; ++populationIndex) {
        childPopulation[populationIndex] = nullptr;
    }
}

/*
 * Genetic Algorithm Destructor.
 */
GeneticAlgorithm::~GeneticAlgorithm() {
    for (int i = 0; i < sizeOfPopulation; ++i) {
        delete[] parentPopulation[i];
    }
    delete[] parentPopulation;
    for (int i = 0; i < sizeOfPopulation+sizeOfPopulation; ++i) {
        delete[] childPopulation[i];
    }
    delete[] childPopulation;
    delete LS;
}

/*
 * Updates the best route by calling generate tour.
 */
void GeneticAlgorithm::checkSolution() {
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

    //Local Search to improve starting population.
    LS->randomPheromoneLocalSearchWithTwoOpt(route);
}

/*
 * Clustered ACO for optimal starting population generation.
 */
int* GeneticAlgorithm::getCACO(){
    srand (rand());
    KMeansClustering::createClusters(4);
    //Minimal parameters chosen as optimality of route was not important, but runtime was
    int numAnts= 3, iterations = 5, probabilityArraySize = 2, twoOptIteration = 3,randomSearchIteration = 3;
    double pheromoneDecrease = 0.9, Q = 1,alpha = 0.6, beta=2.1;
    auto* a = new CACO(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta,twoOptIteration,randomSearchIteration);

    a->optimize(iterations);

    int* temp = a->returnResults();

    //Local search used to locate optimal route.
    LS->twoOptLocalPheromoneAddonSearch(temp); //BEST
//    LS->randomPheromoneLocalSearchWithTwoOpt(temp);
//    LS->LKSearch(temp);
    delete a;
    KMeansClustering::freeClusters();
    return temp;
}

/*
 * Creates a random population of children which are then selected to be parents.
 */
void GeneticAlgorithm::generateStartingPopulation(int type,int selectType) {
    //Uncomment to use the different populations.
    /*
     * Random Starting Population.
     */
    if(type == 1) {
        for (int populationIndex = 0; populationIndex < sizeOfPopulation + sizeOfPopulation; ++populationIndex) {
            childPopulation[populationIndex] = new int[NUM_OF_CUSTOMERS + 1];
            randomRoute(childPopulation[populationIndex]);
            childPopulationCounter++;
        }
        selectChildrenForParents(selectType);
    }
    else if(type == 2) {
        /*
         * Clustered ACO starting population.
         */
        for (int i = 0; i < sizeOfPopulation; ++i) {
            parentPopulation[i] = getCACO();
        }
    }
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
    for (int popCounter = end-1; popCounter >= begin; --popCounter) {
        delete[] population[popCounter];
    }
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
void GeneticAlgorithm::runGenerations(int selection,int crossover,int mutation) {
    for (int x = 1; x <= generations; ++x) {
        childPopulationCounter = 0;
        crossoverOperator(crossover);
        if(mutation != 0)
            randomMutateChildren(mutation);
        selectChildrenForParents(selection);
        addRunDataToFile(x,best_sol->tour_length);
        //repairParents();
    }
}

/*
 * Calls the different crossover operators to generate children from the parents.
 */
void GeneticAlgorithm::crossoverOperator(int type) {
    childPopulationCounter = 0;
    auto iterations = LS->getIterations();
    for (int recombineCounter = 1; recombineCounter < sizeOfPopulation; ++recombineCounter) {
        int** tempChildren;
        //SELECT CROSSOVER OPERATOR BY UNCOMMENTING.
        //GPX:
        if(type == 1){
            //PMX:
            tempChildren = CrossoverOperators::partiallyMappedCrossover(parentPopulation[0],parentPopulation[recombineCounter]);
        }
        else if (type == 2){
            //PX:
            tempChildren = CrossoverOperators::PCRecombine(parentPopulation[0], parentPopulation[recombineCounter],iterations->first,iterations->second);
        }
//        int** tempChildren = CrossoverOperators::GPCRecombine(parentPopulation[0], parentPopulation[recombineCounter],iterations->first,iterations->second);
        //PX:
        //Test Recombination -- Debugging:
//      int** tempChildren = CrossoverOperators::testRecombination(parentPopulation[0],parentPopulation[recombineCounter]);


        //Add the generated children to the children population.
        childPopulation[childPopulationCounter] = new int[NUM_OF_CUSTOMERS+1];
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
            childPopulation[childPopulationCounter][i] = tempChildren[0][i];
        }
        childPopulationCounter++;

        if(tempChildren[1][0] != INT_MAX) {
            childPopulation[childPopulationCounter] = new int[NUM_OF_CUSTOMERS+1];
            for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
                childPopulation[childPopulationCounter][i] = tempChildren[1][i];
            }
            childPopulationCounter++;
        }


        delete[] tempChildren[0]; delete[] tempChildren[1]; delete[] tempChildren;
    }
    delete iterations;
    if(childPopulationCounter < sizeOfPopulation) {
        childPopulation[childPopulationCounter++] = getCACO();
    }
}

/*
 * Uses a selection operator to select viable children to be parents.
 */
void GeneticAlgorithm::selectChildrenForParents(int type) {
    for (int i = 0; i < sizeOfPopulation; ++i) {
        delete[] parentPopulation[i];
        parentPopulation[i] = nullptr;
    }
    int** tempParentPopulation;
    if(type == 1){
        //Truncation Selection:
        tempParentPopulation = Selection::greedySelection(childPopulation,childPopulationCounter,sizeOfPopulation);
    }
    else if(type == 2){
        //Correlated family-based Selection:
        tempParentPopulation = Selection::correlativeFamilyBasedSelection(childPopulation,childPopulationCounter,sizeOfPopulation);
    }


    for (int i = 0; i < sizeOfPopulation; ++i) {
        parentPopulation[i] = new int[NUM_OF_CUSTOMERS+1];
        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
            parentPopulation[i][j] = tempParentPopulation[i][j];
        }
        delete[] tempParentPopulation[i];
    }
    delete[] tempParentPopulation;

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
void GeneticAlgorithm::randomMutateChildren(int type){
    for (int i = 0; i < childPopulationCounter; ++i) {
        int toMutate = rand()%probabiltyOfMutation;
        if(toMutate == 0) {
            //Mutation Operators, uncomment to select different operators.
            if(type == 1){
                //Random Swap Mutation:
                Mutation::randomSwapMutation(childPopulation[i]);
            }
            else if (type == 2){
                //Local Search Mutation:
                LS->randomPheromoneLocalSearchWithTwoOpt(childPopulation[i]);
            }

            //Random Swap Mutation:
//            Mutation::randomSwapMutation(childPopulation[i]);
            //Lin-Kernighan Mutation:
//            Mutation::LKMutation(childPopulation[i],LS);
        }
    }
}
