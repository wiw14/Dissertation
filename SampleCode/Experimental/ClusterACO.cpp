#include "ClusterACO.h"

#include <climits>
#include <functional>
#include<string>

/*
 * ================================================================================ *
 * AntColonyOptimisation FILE METHODS
 * ================================================================================ *
 */

//FILE* ACOFile;
//
//void openACOFile(){
//    if ((ACOFile = fopen(R"(C:\Users\wmw13\Documents\GitHub\Dissertation\SampleCode\storeACO.csv)","a")) == NULL) { printf("ERROR\n");}
//    fprintf(ACOFile," , , \n");
//}
//
//void closeACOFile(){
//    fclose(ACOFile);
//}
//
//void addLocalOptimumToFile(double localOtimum, int iteration, int ant){
//    fprintf(ACOFile,"%d,%d,%f\n",iteration,ant,localOtimum);
//}

/*
 * ================================================================================ *
 * ANT COLONY OPTIMISATION
 * ================================================================================ *
 */

/*
 * Ant Colony Optimization (AntColonyOptimisation) constructor, sets the instance variables needed in the configuration of AntColonyOptimisation.
 */
ClusterACO::ClusterACO(int numberOfAnts, double pheromoneDecreaseFactor, double q, int ProbabilityArraySize, double Alpha,
         double Beta, int TwoOptIteration, int RandomSearchIteration, int* Alphabet, int AlphabetSize) {
    //Creates a local search object to allow local searches to be used after a path has been found.
    //LS = new localSearch(RandomSearchIteration, TwoOptIteration);

    //Sets the current best path length to infinity (Max number int can store).
    bestRouteLength = (double) INT_MAX;

    alphabet = Alphabet;
    sizeOfAlphabet = AlphabetSize;

    /*
     * ACO: x : {0,1,2,..,sizeOfAlphabet-1}.
     * Alphabet[x] = customer.
     */

    //Sets the needed parameters for AntColonyOptimisation configuration.
    numOfAnts = numberOfAnts;
    pheromoneDecrease = pheromoneDecreaseFactor;
    Q = q;
    alpha = Alpha;
    beta = Beta;
    probabilitySize = ProbabilityArraySize;

    //Creates and instantiates probability array.
    //Used to Store the probability of choosing a next customer.
    probability = new double *[probabilitySize];
    for (int index = 0; index < probabilitySize; index++)
        probability[index] = new double[2];
    resetProbability();

    //Instantiates pheromones to a random integer.
    for (int i = 0; i < sizeOfAlphabet; i++) {
        for (int j = i + 1; j < sizeOfAlphabet; j++) {
            pheromones[getArcCode(i, j)] = distribution(seed);
        }
    }


    //Creates and instantiates the route arrays which are used to store a route
    //while processing; and the best possible found route.
    routes = new int *[numberOfAnts];
    bestRoute = new int[sizeOfAlphabet];
    for (int ant = 0; ant < numberOfAnts; ant++) {
        routes[ant] = new int[sizeOfAlphabet];
        for (int customer = 0; customer < sizeOfAlphabet; customer++) {
            routes[ant][customer] = -1;
            bestRoute[customer] = -1;
        }
    }
    //Opens AntColonyOptimisation file to store optimas.
//    openACOFile();
}

/*
 * Function is used to get the index used to access the pheromones map.
 * It takes two customer as input and returns the string index of the pheromone of the arc
 * between the two customers.
 */
std::string ClusterACO::getArcCode(int customerA, int customerB) {
    //Utilises std::string to build a string.
    std::string output;
    std::string CustomerA = std::to_string(customerA);
    std::string CustomerB = std::to_string(customerB);
    //The index string for the arc between A and B is the same as between B and A.
    if (customerA < customerB) {
        output.append(CustomerA);
        output.append(" ");
        output.append(CustomerB);
    } else {
        output.append(CustomerB);
        output.append(" ");
        output.append(CustomerA);
    }
    return output;
}

/*
 * AntColonyOptimisation destructor, frees all used arrays, as well as freeing the local search object.
 */
ClusterACO::~ClusterACO() {
    //Deletes local search object.
    //delete LS;

    //Frees multi-dimensional arrays.
    for (int ant = 0; ant < numOfAnts; ant++)
        delete[] routes[ant];
    for (int prob = 0; prob < probabilitySize; ++prob)
        delete[] probability[prob];
    delete[] routes;
    delete[] probability;
    delete[] bestRoute;

    //Closes AntColonyOptimisation file after it has been used.
//    closeACOFile();
}

/*
 * Resets the route for the inputted ant all to -1.
 */
void ClusterACO::resetRoute(int ant) {
    for (int customer = 0; customer < sizeOfAlphabet; customer++)
        routes[ant][customer] = -1;
}

/*
 * Resets all the probabilities to -1.
 */
void ClusterACO::resetProbability() {
    for (int index = 0; index < probabilitySize; index++) {
        probability[index][0] = -1;
        probability[index][1] = -1;
    }
}

/*
 * Iterates for specified number of iterations, then for each ant it finds a route and
 * evaluates the route. If the route is better it updates the current best.
 */
void ClusterACO::optimize(int iterations) {
    int improve = 0;
    while(improve < iterations){
//    for (int iter = 1; iter <= iterations; iter++) {
        for (int ant = 0; ant < numOfAnts; ant++) {
            //While the route for the ant isn't valid.
            //The route is reset and re-calculated.
            while (valid(ant)) {
                resetRoute(ant);
                route(ant);
            }

            //Calculate the length of the route.
            double routeLength = length(ant);

            //If the new route is shorter than the current best it updates the best.
            if (routeLength <
                bestRouteLength) {
                improve = 0;
                bestRouteLength = routeLength;
                for (int customer = 0; customer < sizeOfAlphabet; customer++)
                    bestRoute[customer] = routes[ant][customer];
            }


        }
        improve++;
        //Pheromones are updated with the new found routes.
        updatePheromones(0,iterations);


        //Resets the all the routes.
        for (int ant = 0; ant < numOfAnts; ant++)
            resetRoute(ant);
        /*
         * LOCAL SEARCH EVERY ITERATION OF THE ANTS.
         */
//        LS->randomPheromoneLocalSearchWithTwoOpt(bestRoute);
        //LS->randomLocalSearch();
        //LS->twoOptLocalSearch(bestRoute);
    }
    /*
     * LOCAL SEARCH AFTER THE ITERATIONS.
     */
    //LS->randomPheromoneLocalSearchWithTwoOpt(bestRoute);
//    LS->randomLocalSearch();
    //LS->twoOptLocalSearch(bestRoute);
}

/*
 * Function calculates the amount of pheromones to add to an edge.
 * Based on route length.
 */
double ClusterACO::amountOfPheromone(double routeLength) const {
    return Q / (routeLength);
}

/*
 * Iterates over all the ants, updating the pheromones for the customers used in the route.
 */
void ClusterACO::updatePheromones(int iterations,int maxIterations) {
    for (int ant = 0; ant < numOfAnts; ant++) {
        double routeLength = length(ant);

        //Decreases all the pheromones by a constant factor.
        for (int i = 0; i < sizeOfAlphabet; i++) {
            for (int j = i + 1; j < sizeOfAlphabet; j++) {
                pheromones[getArcCode(i, j)] = pheromones[getArcCode(i, j)] * pheromoneDecrease;
            }
        }

        //Run local search to improve the route before updating the pheromones.
//        LS->LKSearch(routes[ant]);
//            LS->randomPheromoneLocalSearchWithTwoOpt(routes[ant]);
//            LS->randomLocalSearch(routes[ant]);
//        LS->randomPheromoneLocalSearch(routes[ant]);
//        LS->twoOptLocalSearch(routes[ant]);

        //For visualisation
//        addLocalOptimumToFile(LS->getRouteLength(routes[ant]),iterations,ant);

        //Update the pheromones of the customers in the route from the local search.
        for (int index = 0; index < sizeOfAlphabet-1; index++) {
            int customerA = routes[ant][index], customerB = routes[ant][index + 1];
            pheromones[getArcCode(customerA, customerB)] += amountOfPheromone(routeLength);
        }
    }

}

/*
 * Finds customers with the largest probability, then selects one at random.
 */
int ClusterACO::getNextCustomer() {
    int count = 0;
    for (int index = 0; index < probabilitySize; index++) {
        if (probability[index][1] != -1)
            count++;
    }

    std::uniform_int_distribution<int> nextCustomerSelector(0, count - 1);
    int nextCustomer = nextCustomerSelector(seed);

    return (int) probability[nextCustomer][1];
}

/*
 * This function gets the probability for choosing the next customer using a heuristic.
 */
double ClusterACO::getProbability(int customerA, int customerB, int ant) {
    double pheromone = pheromones[getArcCode(customerA, customerB)];
    double distance = get_distance(alphabet[customerA], alphabet[customerB]);

    double sum = 0.0;
    for (int customer = 0; customer < sizeOfAlphabet; customer++) {
        if (ClusterACO::exists(customerA, customer)) {
            if (!visited(ant, customer)) {
                auto ETA = (double) pow(1 / (get_distance(alphabet[customerA], alphabet[customer])), beta);
                double TAU = (double) pow(pheromones[getArcCode(customerA, customer)], alpha);
                sum += ETA * TAU;
            }
        }
    }


    return (((double) pow(pheromone, alpha)) * ((double) pow(1 / distance, beta))) / sum;
}

/*
 * Generates a route from the DEPOT which can be then evaluated in the optimise function.
 */
void ClusterACO::route(int ant) {
    // Start route at depot
    routes[ant][0] = DEPOT;

    //loop through all but one customer
    for (int i = 0; i < sizeOfAlphabet-1; i++) {
        int customerA = routes[ant][i];

        //loop through all the customers to look for connections
        for (int customerB = 0; customerB < sizeOfAlphabet; customerB++) {
            //If the customerA is the same as the customer selected skip.
            if (customerA == customerB) {
                continue;
            }
            //Checks if a path exists between the customers.
            if (ClusterACO::exists(customerA, customerB)) {
                //Checks whether the customer has already been visited.
                if (!visited(ant, customerB)) {
                    //Calculates the probability from A to B, if its better than the current probability
                    //set the current next customer to B.
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

        //Gets the next customer based on the calculated probabilities.
        int nextCustomer = getNextCustomer();
        //Checks for deadlock.
        if (nextCustomer == -1)
            return;

        //Sets the next customer and resets the probabilities.
        routes[ant][i + 1] = nextCustomer;
        resetProbability();
    }
}

/*
 * Checks whether then is an arc from A to B.
 */
bool ClusterACO::exists(int customerA, int customerB) {
    return (get_distance(alphabet[customerA], alphabet[customerB]) > 0);
}

/*
 * Determines whether a route is valid, essentially the termination criteria for route searching.
 */
bool ClusterACO::valid(int ant) {
    for (int i = 0; i < sizeOfAlphabet-1; i++) {
        //Checks customers are valid.
        int customerA = routes[ant][i];
        int customerB = routes[ant][i + 1];
        if (customerA < 0 || customerB < 0) {
            return true;
        }
        //Checks that there is an arc between the customers.
        if (!ClusterACO::exists(customerA, customerB)) {
            return true;
        }
        //Checks that none of the customers appear in duplicate within the route.
        for (int j = 0; j < i - 1; j++) {
            if (routes[ant][i] == routes[ant][j]) {
                return true;
            }
        }
    }

    //Checks that there is an arc from the last customer to the DEPOT.
    if (!ClusterACO::exists(DEPOT, routes[ant][sizeOfAlphabet-1])) {
        return true;
    }


    //If all the checks are satisfied then the function returns false
    //signifying the route inputted is valid.
    return false;
}

/*
 * Checks whether the customer has already been visited by the current ant.
 */
bool ClusterACO::visited(int ant, int customer) {
    for (int index = 0; index < sizeOfAlphabet; index++) {
        //No customer at that index
        if (routes[ant][index] == -1)
            break;
        //Found customer
        if (routes[ant][index] == customer)
            return true;
    }
    return false;
}

/*
 * Gets the total length of the current route.
 * Utilises get_distance() method which is part of the sample framework for the competition.
 */
double ClusterACO::length(int ant) {
    double total_length = 0.0;
    for (int customer = 0; customer < sizeOfAlphabet-1; customer++) {
        total_length += get_distance(alphabet[routes[ant][customer]], alphabet[routes[ant][customer + 1]]);
    }
    return total_length;
}

/*
 * Getter method, returns the current best route stored in the object for use in other class files.
 */
int *ClusterACO::returnResults() {
    int* configBestRoute = new int[sizeOfAlphabet];
    for (int routeIndex = 0; routeIndex < sizeOfAlphabet; ++routeIndex)
        configBestRoute[routeIndex] = alphabet[bestRoute[routeIndex]];
    return configBestRoute;
}

/*
 * Uses the generate tour function within LocalSearch to calculate the total distance
 * including charging stations and depots.
 */
//double ClusterACO::getRL(int *route) {
//    return GenerateTour::getRouteLength(route);
//}

