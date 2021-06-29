#include "KNN.h"

/*
 * ================================================================================ *
 * RANDOM KNN PATH SEARCH
 * ================================================================================ *
 */

using namespace std;

/*
 * Finds numOfClusters nearest neighbours to a customer and randomly picks one from the generated list.
 * The random nature aims to improve upon greedy euclidean heuristic by forcing movement from local optimums.
 */
KNN::KNN(int KNN,int randomSearchIteration,int twoOptIteration) {
    //Create Local Search object to improve the route.
    LS = new localSearch(randomSearchIteration,twoOptIteration);

    //Instantiate needed arrays.
    v = new int[NUM_OF_CUSTOMERS+1];
    filter = new int[NUM_OF_CUSTOMERS+1];
    neighbours = new int*[NUM_OF_CUSTOMERS+1];

    //Set default values for arrays.
    for (int index = 0; index <= NUM_OF_CUSTOMERS ; ++index) {
        filter[index] = 0;
        v[index] = -1;
    }

    //Generates the numOfClusters nearest neighbours for all the customers.
    for (int customer = 0; customer <= NUM_OF_CUSTOMERS; ++customer) {
        neighbours[customer] = getKNN(customer,KNN);
    }

    //Generates the route using the numOfClusters nearest neighbours.
    generateRoute(KNN);

    //Run local searches.
    LS->randomPheromoneLocalSearchWithTwoOpt(v);
    //LS->randomPheromoneLocalSearch(v);
    //LS->randomLocalSearch(v);
    //LS->twoOptLocalSearch(v);
    GenerateTour::getRouteLength(v);

    //Checks the reverse order for KNN route for improvements.
    int* vReverse = new int[NUM_OF_CUSTOMERS+1];
    int tempIndex = NUM_OF_CUSTOMERS;
    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index)
        vReverse[index]=v[tempIndex--];
    GenerateTour::getRouteLength(vReverse);

    //De-allocates memory.
    delete[]vReverse;
    delete LS;
}


/*
 * Locates the numOfClusters nearest neighbours to the inputted customer.
 */
int * KNN::getKNN(int customer, int KNN) {
    int *neighbour = new int[KNN];
    for (int index = 0; index < KNN; ++index)
        neighbour[index] = -1;
    for (int newCustomer = 0; newCustomer <= NUM_OF_CUSTOMERS; ++newCustomer) {
        if (customer != newCustomer) {
            for (int KNNIndex = 0; KNNIndex < KNN; ++KNNIndex) {
                if(neighbour[KNNIndex] == -1){
                    neighbour[KNNIndex] = newCustomer;
                }
                else if (get_distance(customer, newCustomer) < get_distance(customer, neighbour[KNNIndex])) {
                    neighbour[KNNIndex] = newCustomer;
                    break;
                }
            }
        }
    }

    return neighbour;
}

/*
 * Destructor to de-allocate the memory used for the arrays.
 */
KNN::~KNN(){
    for (int index = 0; index <= NUM_OF_CUSTOMERS ; ++index)
        delete[] neighbours[index];
    delete[] v;
    delete[] neighbours;
    delete[] filter;
    delete LS;
}

/*
 * Loops through all the found KNNs randomly chooses one from the list.
 * If the chosen KNN has already been visited, loop through KNN finding the closest non-visited customer.
 * If all have been visited find the next closest customer.
 */
void KNN::generateRoute(int KNN){
    int index;
    int next;
    v[0] = 0;
    filter[0] = 1;
    for (int customer = 1; customer <= NUM_OF_CUSTOMERS+1; ++customer) {
        index = rand()%(KNN-1);
        next = neighbours[v[customer - 1]][index];
        if(filter[next] == 0) {
            filter[next] = 1;
            v[customer] = next;

        }
        else{
            for (int i = 0; i < KNN; ++i) {
                next = neighbours[v[customer - 1]][i];
                if(filter[next] == 0){
                    filter[next] = 1;
                    v[customer] = next;
                    break;
                }
            }
            if(v[customer] == -1){
                next = findNextClosestNode(customer-1);
                v[customer] = next;
                filter[next] = 1;
            }
        }
    }

}

/*
 * Loops through all the customers and finds the next closest customer which hasn't been visited.
 */
int KNN::findNextClosestNode(int customer){
    int currentBest = INT_MAX;
    for (int newCustomer = 0; newCustomer <= NUM_OF_CUSTOMERS; ++newCustomer) {
        if(newCustomer != customer){
            if (filter[newCustomer] == 0 && get_distance(customer, newCustomer) < currentBest){
                return newCustomer;
            }
        }
    }
    return customer;
}

/*
 * Displays the route to the command line.
 */
void KNN::printRoute(){
    for (int customer = 0; customer <=NUM_OF_CUSTOMERS; ++customer)
        printf("%d, ", v[customer]);
    printf("\n");
}
