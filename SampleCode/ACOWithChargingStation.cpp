#include <climits>
#include <functional>
#include "ACOWithChargingStation.h"


/*
 * ================================================================================ *
 * ANT COLONY OPTIMISATION
 * ================================================================================ *
 */
/*
 * Constructor for the ACO, initialises all the necessary values.
 */
ACOCS::ACOCS(int numberOfAnts, double pheromoneDecreaseFactor, double q, int ProbabilityArraySize, double Alpha,
           double Beta, int TwoOptIteration, int RandomSearchIteration) {
    numOfAnts = numberOfAnts;
    bestRouteLength = (double) INT_MAX;
    pheromoneDecrease = pheromoneDecreaseFactor;
    twoOptIterations = TwoOptIteration;
    randomSearchIteration = RandomSearchIteration;
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
            localSearchPheromone[getArcCode(i,j)] = 1;
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

/*
 * Generates a string which references the arc connecting customerA to customerB, to be used as a key in a map.
 */
std::string ACOCS::getArcCode(int customerA, int customerB) {
    char *index = new char[4];
    if (customerA < customerB)
        sprintf(index, "%d %d", customerA, customerB);
    else
        sprintf(index, "%d %d", customerB, customerA);
    std::string arcCode(index);
    delete[] index;
    return arcCode;
}

/*
 * De-constructor ensures all used memory is deallocated.
 */
ACOCS::~ACOCS() {
//    for (iterator = pheromones.begin(); iterator != pheromones.end(); iterator++)
//        pheromones.erase(iterator);
//    for (auto iter = localSearchPheromone.begin(); iter != localSearchPheromone.end(); iter++)
//        localSearchPheromone.erase(iter);
    for (int ant = 0; ant < numOfAnts; ant++)
        delete[] routes[ant];
    for (int prob = 0; prob < probabilitySize; ++prob)
        delete[] probability[prob];
    delete[] routes;
    delete[] probability;
    delete[] bestRoute;
}

/*
 * Clears the route for all the ants.
 */
void ACOCS::resetRoute(int ant) {
    for (int customer = 0; customer <= NUM_OF_CUSTOMERS; customer++)
        routes[ant][customer] = -1;
}

/*
 * Clears all the probabilities for picking certain customers.
 */
void ACOCS::resetProbability() {
    for (int index = 0; index < probabilitySize; index++) {
        probability[index][0] = -1;
        probability[index][1] = -1;
    }
}

/*
 * Tries to find an optimal route by creating a new route each iteration and compares it to the current best solution.
 */
void ACOCS::optimize(int iterations) {
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
    randomPheromoneLocalSearch();
    //randomLocalSearch();
    //twoOptLocalSearch();
}

/*
 * Gets the number of pheromones to increment for a given arc connection.
 */
double ACOCS::amountOfPheromone(double routeLength) const {
    return Q / (routeLength);
}

/*
 * Update pheromones by first decreasing all the pheromones by a constant factor then increasing the pheromones in the current route.
 */
void ACOCS::updatePheromones() {
    for (int ant = 0; ant < numOfAnts; ant++) {
        //double routeLength = length(ant);
        for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) { //Decreases all the pheromones by a constant factor.
            for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
                pheromones[getArcCode(i, j)] = pheromones[getArcCode(i, j)] * pheromoneDecrease;
            }
        }
        for (int index = 0; index < NUM_OF_CUSTOMERS; index++) {
            int customerA = routes[ant][index], customerB = routes[ant][index + 1];
            pheromones[getArcCode(customerA, customerB)] =  amountOfPheromone(getRouteLength(routes[ant]));
        }
    }
}

int ACOCS::getNextCustomer() {
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

double ACOCS::getProbability(int customerA, int customerB, int ant) {
    double pheromone = pheromones[getArcCode(customerA, customerB)];
    double distance = (get_distance(customerA, customerB) * 1) + (get_energy_consumption(customerA, customerB) * 0) +
                      (get_customer_demand(customerB) * 0);

    double sum = 0.0;
    for (int customer = 0; customer <= NUM_OF_CUSTOMERS; customer++) {
        if (ACOCS::exists(customerA, customer)) {
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

void ACOCS::route(int ant) {
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
            if (ACOCS::exists(customerA, customerB)) { //checks if a path exists between the customers.
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

bool ACOCS::exists(int customerA, int customerB) {
    return (get_distance(customerA, customerB) > 0);
}

bool ACOCS::valid(int ant) {
    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        int customerA = routes[ant][i];
        int customerB = routes[ant][i + 1];
        if (customerA < 0 || customerB < 0) {
            return true;
        }
        if (!ACOCS::exists(customerA, customerB)) {
            return true;
        }
        for (int j = 0; j < i - 1; j++) {
            if (routes[ant][i] == routes[ant][j]) {
                return true;
            }
        }
    }

    if (!ACOCS::exists(DEPOT, routes[ant][NUM_OF_CUSTOMERS])) {
        return true;
    }

    return false;
}

bool ACOCS::visited(int ant, int customer) {
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++) {
        if (routes[ant][index] == -1)
            break; //No customer at that index
        if (routes[ant][index] == customer)
            return true; //Found customer
    }
    return false;
}

double ACOCS::length(int ant) {
    double total_length = 0.0;
    for (int customer = 0; customer < NUM_OF_CUSTOMERS; customer++) {
        total_length += get_distance(routes[ant][customer], routes[ant][customer + 1]);
    }
    return total_length;
}

int *ACOCS::returnResults() {
    return bestRoute;
}

/*
 * ================================================================================ *
 * LOCAL SEARCHES
 * ================================================================================ *
 */

/*
 * 2-opt local search.
 */
void ACOCS::twoOptLocalSearch() {
    int improve = 0;
    int* tempRoute = new int[NUM_OF_CUSTOMERS+1];
    while ( improve < twoOptIterations ){
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            tempRoute[index] = bestRoute[index];
        double route_length = ACOCS::getRouteLength(bestRoute);
        for (int i = 0; i < NUM_OF_CUSTOMERS; ++i){
            for (int j = i + 1; j <= NUM_OF_CUSTOMERS ; ++j) {
                //printf("i %d, j %d\n",i,j);
                ACOCS::twoOptSwap(i,j,tempRoute,bestRoute);
                double new_route_length = ACOCS::getRouteLength(tempRoute);
                if (new_route_length < route_length){
                    improve = 0;
                    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
                        bestRoute[index] = tempRoute[index];
                    route_length = new_route_length;
                }
            }
        }
        improve++;
    }
    delete[] tempRoute;
//    for (int i = 0; i <= NUM_OF_CUSTOMERS ; ++i)
//        printf("%d, ",bestRoute[i]);
//    printf("\n");
}

/*
 * Swaps the route between the inputted points used in 2-opt local search.
 */
void ACOCS::twoOptSwap(int i , int j, int* route, const int* currRoute){
    for (int k = 0; k < i; ++k)
        route[k] = currRoute[k];
    int index = i;
    for (int k = j; k >= i; --k) {
        route[index] = currRoute[k];
        index++;
    }
    for (int k = j+1; k <= NUM_OF_CUSTOMERS; ++k) {
        route[k] = currRoute[k];
    }
}

/*
 * 2-opt local search modified to work alongside Pheromone Random Local Search.
 */
void ACOCS::twoOptLocalPheromoneAddonSearch(int* currentRoute) {
    int improve = 0;
    int* tempRoute = new int[NUM_OF_CUSTOMERS+1];
    while ( improve < twoOptIterations ){
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            tempRoute[index] = currentRoute[index];
        double route_length = ACOCS::getRouteLength(currentRoute);
        for (int i = 0; i < NUM_OF_CUSTOMERS; ++i){
            for (int j = i + 1; j <= NUM_OF_CUSTOMERS ; ++j) {
                //printf("i %d, j %d\n",i,j);
                ACOCS::twoOptSwap(i,j,tempRoute,currentRoute);
                double new_route_length = ACOCS::getRouteLength(tempRoute);
                if (new_route_length < route_length){
                    improve = 0;
                    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
                        currentRoute[index] = tempRoute[index];
                    route_length = new_route_length;
                }
            }
        }
        improve++;
    }
    delete[] tempRoute;
//    for (int i = 0; i <= NUM_OF_CUSTOMERS ; ++i)
//        printf("%d, ",bestRoute[i]);
//    printf("\n");
}

/*
 * Decreases all the pheromones being used for the weighted random number generator used in the local search.
 */
void ACOCS::decreaseLocalSearchPheromone() {
    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
            if(localSearchPheromone[getArcCode(i,j)] > 1)
                localSearchPheromone[getArcCode(i,j)] = localSearchPheromone[getArcCode(i,j)] * 0.8;
        }
    }
}

/*
 * Gets the total number of pheromones in the system, used for weighted random number generator.
 */
int ACOCS::getTotalWeight() {
    int totalWeight = 0;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
            totalWeight += localSearchPheromone[getArcCode(i, j)];
        }
    }
    return totalWeight;
}

/*
 * Generates a random number pair (x and y) based on the local search pheromones.
 */
std::vector<int> ACOCS::getRandomNumber() { //type either x (0) or y (1).
    std::vector<int> xy(2);
    int totalWeight = getTotalWeight();
    xy.operator[](0) = -1; xy.operator[](1) = -1;
    //printf("type: %d tw: %d\n",type ,totalWeight); //DEBUGGING
    int val = rand() % (totalWeight - 1);
    totalWeight = 0;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
            totalWeight += localSearchPheromone[getArcCode(i,j)];
            if (val <= (totalWeight)) {
                //printf("%d\n",index); //DEBUGGING
                xy.operator[](0) = i;
                xy.operator[](1) = j;
                return xy;
            }
        }
    }
    return xy;
}

/*
 * Randomly switches two customers in the route, to find an optimal solution.
 */
void ACOCS::randomLocalSearch() {
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    double route_length = ACOCS::getRouteLength(bestRoute);
    double new_route_length = route_length;
    int iters = 0, x, y;
    while (new_route_length >= route_length && iters < randomSearchIteration) {
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
        new_route_length = ACOCS::getRouteLength(tempRoute);
        //DEBUGGING
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            printf("%d, ",tempRoute[index]);
//        printf("\n");
    }
    //Two Opt Local As Well
    //ACO::twoOptLocalPheromoneAddonSearch(tempRoute);
    if (new_route_length < route_length) {
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            bestRoute[index] = tempRoute[index];
    }
    delete[] tempRoute;
}

/*
 * Switches two customers based on a weighted average determined by pheromones.
 */
void ACOCS::randomPheromoneLocalSearch() {
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    double route_length = ACOCS::getRouteLength(bestRoute);
    double new_route_length = route_length;
    int iters = 0, x, y;
    std::vector<int> xy(2);
    while (new_route_length >= route_length && iters < randomSearchIteration) { //(rand()%5)+10)
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            tempRoute[index] = bestRoute[index];
        iters++;
        //x = rand() % NUM_OF_CUSTOMERS;
        //y = ((rand() % 5) + 1 + x);

        xy = getRandomNumber();
        if (xy.operator[](0) < xy.operator[](1)){
            x = xy.operator[](0);
            y = xy.operator[](1);
        }
        else{
            x = xy.operator[](1);
            y = xy.operator[](0);
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
        //2-Opt As well
        //ACOCS::twoOptLocalPheromoneAddonSearch(tempRoute);
        new_route_length = ACOCS::getRouteLength(tempRoute);
        //DEBUGGING
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            printf("%d, ",tempRoute[index]);
//        printf("\n");
    }
    decreaseLocalSearchPheromone();
    if (new_route_length < route_length) {
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            bestRoute[index] = tempRoute[index];
        localSearchPheromone[getArcCode(x,y)] = localSearchPheromone[getArcCode(x,y)] + (int) ((route_length - new_route_length));
    }
    //printLocalSearchPheromones();
    delete[] tempRoute;

}

/*
 * ================================================================================ *
 * GENERATE ROUTE
 * ================================================================================ *
 */
/*
 * Generates a route between charging stations and depot using the route of customers.
 * Returns a value determining the fitness of the inputted route.
 */
double ACOCS::getRouteLength(const int *route) {

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
    int i = 0;
    while (i <= NUM_OF_CUSTOMERS) {
        prev = tour[steps - 1];
        if (route[i] == 0) {
            i++;
            continue;
        }
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

    double route_length = fitness_evaluation(tour,steps);
    if (route_length < best_sol->tour_length){
        for (int index = 0; index < steps; ++index)
            best_sol->tour[index] = tour[index];
        best_sol->steps = steps;
        best_sol->tour_length = route_length;
    }
    return route_length;
}