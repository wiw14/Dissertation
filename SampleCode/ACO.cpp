//
// Created by wmw13 on 11/02/2021.
//

#include <climits>
#include <functional>
#include "ACO.h"

#define SIZEOFPHEROMONES (((NUM_OF_CUSTOMERS+1)*(NUM_OF_CUSTOMERS+1))-(NUM_OF_CUSTOMERS+1))/2

ACO::ACO(int numberOfAnts, double pheromoneDecreaseFactor, double q, int ProbabilityArraySize, double Alpha,
         double Beta) {
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
    localSearchPheromone = new int *[NUM_OF_CUSTOMERS + 1];
    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index) {
        localSearchPheromone[index] = new int[2];
        localSearchPheromone[index][0] = 1;
        localSearchPheromone[index][1] = 1;
    }


}

char *ACO::getArcCode(int customerA, int customerB) {
    char *index = new char[4];
    if (customerA < customerB)
        sprintf(index, "%d %d", customerA, customerB);
    else
        sprintf(index, "%d %d", customerB, customerA);
    return index;
}

void ACO::printPheromones() {
    for (const auto &item : pheromones) {
        printf("%s %f\n", item.first.c_str(), item.second);
    }
}

ACO::~ACO() {
    for (iterator = pheromones.begin(); iterator != pheromones.end(); iterator++)
        pheromones.erase(iterator);
    for (int ant = 0; ant < numOfAnts; ant++)
        delete[] routes[ant];
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
        delete[] localSearchPheromone[index];
    delete[] localSearchPheromone;
    delete[] routes;
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
        randomPheromoneLocalSearch();
        //randomLocalSearch();
        //twoOptLocalSearch();
    }
    /*
     * LOCAL SEARCH AFTER THE ITERATIONS
     */
    //twoOptLocalSearch();
    randomPheromoneLocalSearch();
    //randomLocalSearch();
}

double ACO::getRouteLength(int *route) {

    int steps;
    int *tour = new int[NUM_OF_CUSTOMERS + 1];
    /*
    * Re-Initialise best_sol
    */
    steps = 0;
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
        tour[index] = -1;
    /*
     * Sets the first item in the tour to DEPOT because all routes start at the depot.
     * Increment steps to 1 due to first step was DEPOT.
     */
    tour[0] = DEPOT;
    steps++;

    int prev, next, chargingStation;
    double activeCapacity = 0.0, activeBatteryLevel = 0.0;
    int i = 1;
    while (i <= NUM_OF_CUSTOMERS) {
        prev = tour[steps - 1];
        next = route[i];
        if ((activeCapacity + get_customer_demand(next)) <= MAX_CAPACITY &&
            activeBatteryLevel + get_energy_consumption(prev, next) <= BATTERY_CAPACITY) {
            activeCapacity += get_customer_demand(next);
            activeBatteryLevel += get_energy_consumption(prev, next);
            tour[steps] = next;
            steps++;
            i++;
        } else if ((activeCapacity + get_customer_demand(next)) > MAX_CAPACITY) {
            activeCapacity = 0.0;
            activeBatteryLevel = 0.0;
            tour[steps] = DEPOT;
            steps++;
        } else if (activeBatteryLevel + get_energy_consumption(prev, next) > BATTERY_CAPACITY) {
//            chargingStation = rand() % (ACTUAL_PROBLEM_SIZE - NUM_OF_CUSTOMERS - 1) + NUM_OF_CUSTOMERS + 1;
            chargingStation = NUM_OF_CUSTOMERS + NUM_OF_STATIONS;
            for (int index = NUM_OF_CUSTOMERS + 1; index <= (NUM_OF_CUSTOMERS + NUM_OF_STATIONS); index++) {
                if (get_distance(next, index) < get_distance(next, chargingStation) && is_charging_station(index))
                    chargingStation = index;
            }
            if (is_charging_station(chargingStation)) {
                activeBatteryLevel = 0.0;
                tour[steps] = chargingStation;
                steps++;
            }
        } else {
            activeCapacity = 0.0;
            activeBatteryLevel = 0.0;
            tour[steps] = DEPOT;
            steps++;
        }
    }

    //close EVRP tour to return back to the depot
    if (tour[steps - 1] != DEPOT) {
        tour[steps] = DEPOT;
        steps++;
    }

    return fitness_evaluation(tour, steps);
}

void ACO::twoOptLocalSearch() {
    int firstSelectionUpperBound, secondSelectionUpperBand, iters = 0;
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    double route_length = getRouteLength(bestRoute);
    double new_route_length = route_length;
    while (new_route_length >= route_length && iters < 100) {
        firstSelectionUpperBound = rand() % (int) (NUM_OF_CUSTOMERS / 2);
        secondSelectionUpperBand = (rand() % (int) (NUM_OF_CUSTOMERS / 2)) + firstSelectionUpperBound;
//        printf("FU %d, SU %d\n",firstSelectionUpperBound,secondSelectionUpperBand); //DEBUGGING
        int tempIndex = 0;
        //Stage 1
        for (int index = 0; index < firstSelectionUpperBound; index++)
            tempRoute[tempIndex++] = bestRoute[index];
        //Stage 2
        for (int index = secondSelectionUpperBand; index >= firstSelectionUpperBound; index--)
            tempRoute[tempIndex++] = bestRoute[index];
        //Stage 3
        for (int index = secondSelectionUpperBand + 1; index <= NUM_OF_CUSTOMERS; index++)
            tempRoute[tempIndex++] = bestRoute[index];

        iters++;
        new_route_length = getRouteLength(tempRoute);
        printf("Current : %f, New : %f\n", route_length, new_route_length);
    }
    if (new_route_length < route_length) {
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            bestRoute[index] = tempRoute[index];
    }
    delete[] tempRoute;
}

void ACO::decreaseLocalSearchPheromone() {
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++) {
        if (localSearchPheromone[index][0] > 1)
            localSearchPheromone[index][0] = (int) (localSearchPheromone[index][0] * 0.9);
        if (localSearchPheromone[index][1] > 1)
            localSearchPheromone[index][1] = (int) (localSearchPheromone[index][1] * 0.9);
    }
}

int ACO::getTotalWeight(int type) {
    int totalWeight = 0;
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
        totalWeight += localSearchPheromone[index][type];
    return totalWeight;
}

int ACO::getRandomNumber(int type) { //type either x (0) or y (1).
    int totalWeight = getTotalWeight(type);
    //printf("type: %d tw: %d\n",type ,totalWeight); //DEBUGGING
    int val = rand() % (totalWeight - 1);
    totalWeight = 0;
    for (int index = 0; index < NUM_OF_CUSTOMERS; index++) {
        totalWeight += localSearchPheromone[index][type];
        if (val <= (totalWeight)) {
            //printf("%d\n",index); //DEBUGGING
            return index;
        }
    }
    return -1;
}

void ACO::randomLocalSearch() {
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    double route_length = getRouteLength(bestRoute);
    double new_route_length = route_length;
    int iters = 0, x, y;
    while (new_route_length >= route_length && iters < 100) {
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            tempRoute[index] = bestRoute[index];
        iters++;
        x = rand() % NUM_OF_CUSTOMERS;
        y = ((rand() % 5) + 1 + x);
        if (y > NUM_OF_CUSTOMERS) {
            y = NUM_OF_CUSTOMERS;
        }
        //printf("X = %d, Y = %d\n", x, y); //DEBUGGING

        int size = (y - x) + 1;
        int *routeTemp = new int[size];
        for (int index = 0; index < size; index++)
            routeTemp[index] = -1;
        int tempIndex = 0;

        for (int index = y; index >= x; index--) {
            routeTemp[tempIndex] = tempRoute[index];
            tempIndex++;
        }
        tempIndex = 0;
        for (int index = x; index <= y; index++) {
            tempRoute[index] = routeTemp[tempIndex];
            tempIndex++;
        }
        delete[] routeTemp;
        new_route_length = getRouteLength(tempRoute);
        //DEBUGGING
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            printf("%d, ",tempRoute[index]);
//        printf("\n");
    }

    if (new_route_length < route_length) {
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            bestRoute[index] = tempRoute[index];
    }
    delete[] tempRoute;
}

void ACO::randomPheromoneLocalSearch() {
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    double route_length = getRouteLength(bestRoute);
    double new_route_length = route_length;
    int iters = 0, x, y;
    while (new_route_length >= route_length && iters < 100) {
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            tempRoute[index] = bestRoute[index];
        iters++;
        //x = rand() % NUM_OF_CUSTOMERS;
        //y = ((rand() % 5) + 1 + x);

        x = getRandomNumber(0);
        y = ((rand() % 5) + 1 + x);
        if (y > NUM_OF_CUSTOMERS) {
            y = NUM_OF_CUSTOMERS;
        }
//        y = getRandomNumber(1);
//        int tempx;
//        if (y < x) {
//            tempx = x;
//            x = y;
//            y = tempx;
//        }
        //printf("X = %d, Y = %d\n", x, y); //DEBUGGING

        int size = (y - x) + 1;
        int *routeTemp = new int[size];
        for (int index = 0; index < size; index++)
            routeTemp[index] = -1;
        int tempIndex = 0;

        for (int index = y; index >= x; index--) {
            routeTemp[tempIndex] = tempRoute[index];
            tempIndex++;
        }
        tempIndex = 0;
        for (int index = x; index <= y; index++) {
            tempRoute[index] = routeTemp[tempIndex];
            tempIndex++;
        }
        delete[] routeTemp;
        new_route_length = getRouteLength(tempRoute);
        //DEBUGGING
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            printf("%d, ",tempRoute[index]);
//        printf("\n");
    }
    decreaseLocalSearchPheromone();
    if (new_route_length < route_length) {

        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            bestRoute[index] = tempRoute[index];
        localSearchPheromone[x][0] = localSearchPheromone[x][0] + (int) ((route_length - new_route_length));
        localSearchPheromone[y][1] = localSearchPheromone[y][1] + (int) ((route_length - new_route_length));
    }
    //printLocalSearchPheromones();
    delete[] tempRoute;
}

void ACO::printLocalSearchPheromones() {
    printf("Pheromones for X : ");
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
        printf("%d, ", localSearchPheromone[index][0]);
    printf("\n");
    printf("Pheromones for Y : ");
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
        printf("%d, ", localSearchPheromone[index][1]);
    printf("\n");
}

double ACO::amountOfPheromone(double routeLength, int customerA, int customerB) {
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
            pheromones[getArcCode(customerA, customerB)] = amountOfPheromone(routeLength, customerA, customerB);
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
        if (exists(customerA, customer)) {
            if (!visited(ant, customer)) {
                double ETA = (double) pow(1 / ((get_distance(customerA, customer) * 1) +
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
            if (exists(customerA, customerB)) { //checks if a path exists between the customers.
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
        if (!exists(customerA, customerB)) {
            return true;
        }
        for (int j = 0; j < i - 1; j++) {
            if (routes[ant][i] == routes[ant][j]) {
                return true;
            }
        }
    }

    if (!exists(DEPOT, routes[ant][NUM_OF_CUSTOMERS])) {
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

