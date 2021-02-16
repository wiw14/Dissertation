//
// Created by wmw13 on 11/02/2021.
//

#include <climits>
#include <functional>
#include "ACO.h"


ACO::ACO(int numberOfAnts, double pheromoneDecreaseFactor, double q, int ProbabilityArraySize, double Alpha,
         double Beta, int TwoOptIteration, int RandomSearchIteration) {
    LS = new localSearch(RandomSearchIteration,TwoOptIteration);
    numOfAnts = numberOfAnts;
    bestRouteLength = (double) INT_MAX;
    pheromoneDecrease = pheromoneDecreaseFactor;
    Q = q;
    alpha = Alpha;
    beta = Beta;
    probabilitySize = ProbabilityArraySize;
    probability = new double *[probabilitySize];
    for (int index = 0; index < probabilitySize; index++)
        probability[index] = new double[2];
    resetProbability();

    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
            pheromones[getArcCode(i, j)] = distribution(seed);
        }
    }

    routes = new int *[numberOfAnts];
    bestRoute = new int[NUM_OF_CUSTOMERS + 1];
    for (int ant = 0; ant < numberOfAnts; ant++) {
        routes[ant] = new int[NUM_OF_CUSTOMERS + 1];
        for (int customer = 0; customer <= NUM_OF_CUSTOMERS; customer++) {
            routes[ant][customer] = -1;
            bestRoute[customer] = -1;
        }
    }

}

std::string ACO::getArcCode(int customerA, int customerB) {
    char *index = new char[4];
    if (customerA < customerB)
        sprintf(index, "%d %d", customerA, customerB);
    else
        sprintf(index, "%d %d", customerB, customerA);
    std::string arcCode(index);
    delete[] index;
    return arcCode;
}

ACO::~ACO() {
    delete LS;
    for (int ant = 0; ant < numOfAnts; ant++)
        delete[] routes[ant];
    for (int prob = 0; prob < probabilitySize; ++prob)
        delete[] probability[prob];
    delete[] routes;
    delete[] probability;
    delete[] bestRoute;
}

void ACO::resetRoute(int ant) {
    for (int customer = 0; customer <= NUM_OF_CUSTOMERS; customer++)
        routes[ant][customer] = -1;
}

void ACO::resetProbability() {
    for (int index = 0; index < probabilitySize; index++) {
        probability[index][0] = -1;
        probability[index][1] = -1;
    }
}

void ACO::optimize(int iterations) {
    for (int iter = 1; iter <= iterations; iter++) {
        //printf("Iteration %d\n",iter); //DEBUGGING
        for (int ant = 0; ant < numOfAnts; ant++) {
            //printf("Ant : %d\n",ant); //DEBUGGING
            while (valid(ant)) {
                resetRoute(ant);
                //printf("Route Reset\n"); //DEBUGGING
                route(ant);
                //printf("Created Route\n"); //DEBUGGING
            }
            //printf("Found Route\n"); //DEBUGGING
            double routeLength = length(ant);

            if (routeLength <
                bestRouteLength) { //If the new route is shorter than the current best it updates the best.
                bestRouteLength = routeLength;
                for (int customer = 0; customer <= NUM_OF_CUSTOMERS; customer++)
                    bestRoute[customer] = routes[ant][customer];
            }

        }
        updatePheromones();
        for (int ant = 0; ant < numOfAnts; ant++)
            resetRoute(ant); //Resets the all the routes;
        /*
         * LOCAL SEARCH EVERY ITERATION OF THE ANTS
         */
        //LS->randomPheromoneLocalSearchWithTwoOpt(bestRoute);
        //LS->randomLocalSearch();
        //LS->twoOptLocalSearch();
    }
    /*
     * LOCAL SEARCH AFTER THE ITERATIONS
     */
    //LS->randomPheromoneLocalSearchWithTwoOpt(bestRoute);
    //LS->randomLocalSearch();
    //LS->twoOptLocalSearch();
}

double ACO::amountOfPheromone(double routeLength) const {
    return Q / (routeLength);
}

void ACO::updatePheromones() {
    for (int ant = 0; ant < numOfAnts; ant++) {
        double routeLength = length(ant);
        for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) { //Decreases all the pheromones by a constant factor.
            for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
                pheromones[getArcCode(i, j)] = pheromones[getArcCode(i, j)] * pheromoneDecrease;
            }
        }
        for (int index = 0; index < NUM_OF_CUSTOMERS; index++) {
            int customerA = routes[ant][index], customerB = routes[ant][index + 1];
            pheromones[getArcCode(customerA, customerB)] = amountOfPheromone(routeLength);
        }
    }
}

int ACO::getNextCustomer() {
    /*
     * Find the 3 largest probabilities then randomly select one.
     */
    int count = 0;
    for (int index = 0; index < probabilitySize; index++) {
        if (probability[index][1] != -1)
            count++;
    }

    std::uniform_int_distribution<int> nextCustomerSelector(0, count - 1);
    int nextCustomer = nextCustomerSelector(seed);
    //printf("Random Selection %d\n",nextCustomer); //DEBUGGING
    return (int) probability[nextCustomer][1];

}

/*
 * NEEDS CHANGING.
 * This function gets the probability for choosing the next customer.
 */
double ACO::getProbability(int customerA, int customerB, int ant) {
    double pheromone = pheromones[getArcCode(customerA, customerB)];
    double distance = (get_distance(customerA, customerB) * 1) + (get_energy_consumption(customerA, customerB) * 0) +
                      (get_customer_demand(customerB) * 0);

    double sum = 0.0;
    for (int customer = 0; customer <= NUM_OF_CUSTOMERS; customer++) {
        if (ACO::exists(customerA, customer)) {
            if (!visited(ant, customer)) {
                auto ETA = (double) pow(1 / ((get_distance(customerA, customer) * 1) +
                                               (get_energy_consumption(customerA, customer) * 0) +
                                               (get_customer_demand(customer) * 0)), beta);
                double TAU = (double) pow(pheromones[getArcCode(customerA, customer)], alpha);
                sum += ETA * TAU;
            }
        }
    }

    double prob = (((double) pow(pheromone, alpha)) * ((double) pow(1 / distance, beta))) / sum;

    return prob;
}

void ACO::route(int ant) {
    //printf("Started Creating Route\n"); //DEBUGGING
    routes[ant][0] = DEPOT; // Start route at depot
    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) { //loop through all but one customer
        int customerA = routes[ant][i]; //first customerA is the depot
        //printf("Selected a customer %d\n",customerA); //DEBUGGING
        for (int customerB = 0;
             customerB <= NUM_OF_CUSTOMERS; customerB++) { //loop through all the customers to look for connections
            //printf("Looking at customer %d\n",customerB); //DEBUGGING
            if (customerA == customerB) { //If the customerA is the same as the customer selected skip.
                continue;
            }
            if (ACO::exists(customerA, customerB)) { //checks if a path exists between the customers.
                if (!visited(ant, customerB)) { //checks whether the customer has already been visited
                    double prob = getProbability(customerA, customerB, ant);
                    for (int index = 0; index < probabilitySize; index++) {
                        if (prob > probability[index][0]) {
                            probability[index][0] = prob;
                            probability[index][1] = (double) customerB;
                            break;
                        }
                    }
                }

            }
        }

        int nextCustomer = getNextCustomer();
        if (nextCustomer == -1) //deadlock
            return;

        routes[ant][i + 1] = nextCustomer;
        //printf("Next Customer %d\n", routes[ant][i + 1]); //DEBUGGING
        resetProbability();
    }
}

bool ACO::exists(int customerA, int customerB) {
    return (get_distance(customerA, customerB) > 0);
}

bool ACO::valid(int ant) {
    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        int customerA = routes[ant][i];
        int customerB = routes[ant][i + 1];
        if (customerA < 0 || customerB < 0) {
            return true;
        }
        if (!ACO::exists(customerA, customerB)) {
            return true;
        }
        for (int j = 0; j < i - 1; j++) {
            if (routes[ant][i] == routes[ant][j]) {
                return true;
            }
        }
    }

    if (!ACO::exists(DEPOT, routes[ant][NUM_OF_CUSTOMERS])) {
        return true;
    }

    return false;
}

bool ACO::visited(int ant, int customer) {
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++) {
        if (routes[ant][index] == -1)
            break; //No customer at that index
        if (routes[ant][index] == customer)
            return true; //Found customer
    }
    return false;
}

double ACO::length(int ant) {
    double total_length = 0.0;
    for (int customer = 0; customer < NUM_OF_CUSTOMERS; customer++) {
        total_length += get_distance(routes[ant][customer], routes[ant][customer + 1]);
    }
    return total_length;
}

int *ACO::returnResults() {
    return bestRoute;
}
double ACO::getRL(int* route){
    return LS->getRouteLength(route);
}

