//
// Created by wmw13 on 16/02/2021.
//

#include <climits>
#include "LocalSearches.h"


localSearch::localSearch(int RandomSearchIteration, int TwoOptIterations) {
    randomSearchIteration = RandomSearchIteration;
    twoOptIterations = TwoOptIterations;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
            localSearchPheromone[getArcCode(i, j)] = 1;
        }
    }
}

localSearch::~localSearch() {

}

/*
 * 2-opt local search.
 */
void localSearch::twoOptLocalSearch(int *bestRoute) {
    int improve = 0;
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    while (improve < twoOptIterations) {
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            tempRoute[index] = bestRoute[index];
        double route_length = getRouteLength(bestRoute);
        for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
            for (int j = i + 1; j <= NUM_OF_CUSTOMERS; ++j) {
                //printf("i %d, j %d\n",i,j);
                twoOptSwap(i, j, tempRoute, bestRoute);
                double new_route_length = getRouteLength(tempRoute);
                if (new_route_length < route_length) {
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
void localSearch::twoOptSwap(int i, int j, int *route, const int *currRoute) {
    for (int k = 0; k < i; ++k)
        route[k] = currRoute[k];
    int index = i;
    for (int k = j; k >= i; --k) {
        route[index] = currRoute[k];
        index++;
    }
    for (int k = j + 1; k <= NUM_OF_CUSTOMERS; ++k) {
        route[k] = currRoute[k];
    }
}

/*
 * 2-opt local search modified to work alongside Pheromone Random Local Search.
 */
void localSearch::twoOptLocalPheromoneAddonSearch(int *currentRoute) {
    int improve = 0;
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    while (improve < twoOptIterations) {
        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
            tempRoute[index] = currentRoute[index];
        double route_length = getRouteLength(currentRoute);
        for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
            for (int j = i + 1; j <= NUM_OF_CUSTOMERS; ++j) {
                //printf("i %d, j %d\n",i,j);
                twoOptSwap(i, j, tempRoute, currentRoute);
                double new_route_length = getRouteLength(tempRoute);
                if (new_route_length < route_length) {
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
void localSearch::decreaseLocalSearchPheromone() {
    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
            if (localSearchPheromone[getArcCode(i, j)] > 1)
                localSearchPheromone[getArcCode(i, j)] = localSearchPheromone[getArcCode(i, j)] * 0.8;
        }
    }
}

/*
 * Gets the total number of pheromones in the system, used for weighted random number generator.
 */
int localSearch::getTotalWeight() {
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
std::vector<int> localSearch::getRandomNumber() { //type either x (0) or y (1).
    std::vector<int> xy(2);
    int totalWeight = getTotalWeight();
    xy.operator[](0) = -1;
    xy.operator[](1) = -1;
    //printf("type: %d tw: %d\n",type ,totalWeight); //DEBUGGING
    int val = rand() % (totalWeight - 1);
    totalWeight = 0;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
            totalWeight += localSearchPheromone[getArcCode(i, j)];
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
void localSearch::randomLocalSearch(int *bestRoute) {
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    double route_length = getRouteLength(bestRoute);
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
        //twoOptLocalPheromoneAddonSearch(tempRoute);
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

/*
 * Switches two customers based on a weighted average determined by pheromones.
 */
void localSearch::randomPheromoneLocalSearch(int *bestRoute) {
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    double route_length = getRouteLength(bestRoute);
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
        if (xy.operator[](0) < xy.operator[](1)) {
            x = xy.operator[](0);
            y = xy.operator[](1);
        } else {
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
        localSearchPheromone[getArcCode(x, y)] =
                localSearchPheromone[getArcCode(x, y)] + (int) ((route_length - new_route_length));
    }
    //printLocalSearchPheromones();
    delete[] tempRoute;

}

void localSearch::randomPheromoneLocalSearchWithTwoOpt(int *bestRoute) {
    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
    double route_length = getRouteLength(bestRoute);
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
        if (xy.operator[](0) < xy.operator[](1)) {
            x = xy.operator[](0);
            y = xy.operator[](1);
        } else {
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
        twoOptLocalPheromoneAddonSearch(tempRoute);
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
        localSearchPheromone[getArcCode(x, y)] =
                localSearchPheromone[getArcCode(x, y)] + (int) ((route_length - new_route_length));
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
double localSearch::getRouteLengthImproved(int *route) {

    int steps;
    int *tour = new int[NUM_OF_CUSTOMERS + 1];
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
//        if(rand()%10 == 6){
//            chargingStation = rand() % (ACTUAL_PROBLEM_SIZE - NUM_OF_CUSTOMERS - 1) + NUM_OF_CUSTOMERS + 1;
//            chargingStation = NUM_OF_CUSTOMERS + NUM_OF_STATIONS;
//            for (int index = NUM_OF_CUSTOMERS + 1; index <= (NUM_OF_CUSTOMERS + NUM_OF_STATIONS); index++) {
//                if (get_distance(next, index) < get_distance(next, chargingStation) && is_charging_station(index))
//                    chargingStation = index;
//            }
//            if (is_charging_station(chargingStation)) {
//                activeBatteryLevel = 0.0;
//                tour[steps] = chargingStation;
//                steps++;
//            }
//        }
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
    //check_solution(tour,steps);
    double route_length = fitness_evaluation(tour, steps);
    if (route_length < best_sol->tour_length) {
        for (int index = 0; index < steps; ++index)
            best_sol->tour[index] = tour[index];
        best_sol->steps = steps;
        best_sol->tour_length = route_length;
    }
    return route_length;
}

std::string localSearch::getArcCode(int customerA, int customerB) {
    char *index = new char[4];
    if (customerA < customerB)
        sprintf(index, "%d %d", customerA, customerB);
    else
        sprintf(index, "%d %d", customerB, customerA);
    std::string arcCode(index);
    delete[] index;
    return arcCode;
}
int localSearch::getTotalLoad(int *route, int startCustomer) {
    double total = 0.0;
    int size = 0;

    for (int customers = startCustomer; customers <= NUM_OF_CUSTOMERS; ++customers) {
        if(route[customers] != 0) {
            total += get_customer_demand(route[customers]);
            if (total <= MAX_CAPACITY)
                size++;
            else
                break;
        }
    }
    return size;
}
double localSearch::getTotalEnergyConsumption(int *route, int startCustomer, int upperBound) {
    double total = 0.0;

    for (int customers = startCustomer; customers < upperBound; ++customers) {
        total += get_energy_consumption(route[customers-1],route[customers]);
    }
    return total;
}

int localSearch::findClosestChargingStation(int customer) {
    int chargingStation = NUM_OF_CUSTOMERS + NUM_OF_STATIONS;
    for (int index = NUM_OF_CUSTOMERS + 1; index <= (NUM_OF_CUSTOMERS + NUM_OF_STATIONS); index++) {
        if (get_distance(customer, index) < get_distance(customer, chargingStation) && is_charging_station(index))
            chargingStation = index;
    }
    return chargingStation;
}

void localSearch::findOptimalCS(int* subRoute, int size){
    double* lengths = new double[size];
    for (int i = 0; i < size-1; ++i)
        lengths[i] = get_distance(subRoute[i],findClosestChargingStation(subRoute[i]))+get_distance(findClosestChargingStation(subRoute[i]),subRoute[i+1]);
    double minLength = INT_MAX;
    int customer = -1;
    for (int i = 0; i < size-1; ++i) {
        if(lengths[i] < minLength){
            minLength = lengths[i];
            customer = i;
        }
    }
    double activeEnergy = 0.0;

}

double localSearch::getRouteLength(const int *route) {
    int* tempRoute = new int[NUM_OF_CUSTOMERS], *tour = new int[NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS], tempIndex = 0,step = 0;
    //INITALISE ARRAYS.
    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index) {
        if(route[index] != 0)
            tempRoute[tempIndex++] = route[index];
    }
//    for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
//        printf("%d, ",tempRoute[i]);
//    }printf("\n");
    for (int index = 0; index < NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS; ++index) {
        tour[index] = -1;
    }
    tour[step++] = DEPOT;

    int start = 0;
    int maxDistForCap;
    while (start < NUM_OF_CUSTOMERS) {
        maxDistForCap = getTotalLoad(tempRoute, start);
        double totalEnergy = getTotalEnergyConsumption(tempRoute,start,start+maxDistForCap);

//        int space = (maxDistForCap)/numberOfStations;
//        int dif = 1;
        if (totalEnergy<BATTERY_CAPACITY) {
            for (int i = start; i < start + maxDistForCap && i < NUM_OF_CUSTOMERS; ++i)
                tour[step++] = tempRoute[i];
        }
        else{
            /*
            * FIND OPTIMAL PATH WITH CHARGING STATIONS.
            */
            int* tempTempRoute = new int[maxDistForCap];
            int index = 0;
            for (int i = start; i < start + maxDistForCap; ++i)
                tempTempRoute[index++] = tempRoute[i];
            findOptimalCS(tempTempRoute,index);
            delete[]tempTempRoute;
        }
        tour[step++] = DEPOT;
        start+=maxDistForCap;
//        for (int i = 0; i < step; ++i) {
//            printf("%d, ",tour[i]);
//        }printf("\n");
    }


    //check_solution(tour,step);
    double route_length = fitness_evaluation(tour, step);
    if (route_length < best_sol->tour_length) {
        for (int index = 0; index < step; ++index)
            best_sol->tour[index] = tour[index];
        best_sol->steps = step;
        best_sol->tour_length = route_length;
    }
    delete[] tempRoute;
//    delete[] tour;

    return route_length;
}



//
//std::vector<int>* localSearch::getTotalEnergyConsumption(int *route, int startCustomer, int upperBound) {
//    double total = 0.0;
//    auto* subRoute = new std::vector<int>;
//
//    for (int customers = startCustomer; customers < upperBound; ++customers) {
////        printf("%d\n",route[customers]);
//        if(route[customers] != 0) {
//            total += get_energy_consumption(route[customers - 1], route[customers]);
////            printf("%f tot = %d cap = dis to home %f == %d\n",total,(BATTERY_CAPACITY),get_energy_consumption(route[customers],DEPOT),(BATTERY_CAPACITY-(int)get_energy_consumption(route[customers],DEPOT)));
//            if ((int)total <= (BATTERY_CAPACITY)){
////                if(get_distance(route[customers],findClosestChargingStation(route[customers]))<20 && total > BATTERY_CAPACITY*0.7){
////                    subRoute->insert(subRoute->begin(), route[customers]);
////                    break;
////                }
//                subRoute->insert(subRoute->begin(), route[customers]);}
//            else
//                break;
////            printf("%d^^%d^%d\n",customers,upperBound,route[customers]);
//        }
//    }
//    return subRoute;
//}
//
//int localSearch::getTotalLoad(int *route, int startCustomer) {
//
//    double total = 0.0;
//    int size = 0;
//
//    for (int customers = startCustomer; customers <= NUM_OF_CUSTOMERS; ++customers) {
//        if(route[customers] != 0) {
//            total += get_customer_demand(route[customers]);
//            if (total <= MAX_CAPACITY)
//                size++;
//            else
//                break;
//        }
//    }
////    printf("=+%d\n",size);
//    return size;
//}
//
//
//int localSearch::findClosestChargingStation(int customer) {
//    int chargingStation = NUM_OF_CUSTOMERS + NUM_OF_STATIONS;
//    for (int index = NUM_OF_CUSTOMERS + 1; index <= (NUM_OF_CUSTOMERS + NUM_OF_STATIONS); index++) {
//        if (get_distance(customer, index) < get_distance(customer, chargingStation) && is_charging_station(index))
//            chargingStation = index;
//    }
//    return chargingStation;
//}
//
//double localSearch::getRouteLength(int *route) {
////    printf("Start\n");
//    int steps;
//    int *tour = new int[NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS];
//    steps = 0;
//    for (int index = 0; index <= NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS; index++) {
//        tour[index] = -1;
//    }
//    /*
//     * Sets the first item in the tour to DEPOT because all routes start at the depot.
//     * Increment steps to 1 due to first step was DEPOT.
//     */
//    tour[0] = DEPOT;
//    steps++;
//    int i = 0;
//    std::vector<int>* subRoute;
//    while (i <= NUM_OF_CUSTOMERS) {
//        if (route[i] == 0) {
//            i++;
//            continue;
//        }
//        int j = i;
//        int jUB = i + getTotalLoad(route, i);
//        while (j < jUB) {
////            printf("---%d--%d\n",j,jUB);
//            subRoute = getTotalEnergyConsumption(route, j, jUB);
//            if(subRoute->empty()){
//                j++;
//                continue;}
//            int back = subRoute->front();
//            j += subRoute->size();
////            printf("-*-\n");
//            while(!subRoute->empty()){
//                if(subRoute->back() != DEPOT) {
//                    tour[steps++] = subRoute->back();
////                    printf("%d\n",subRoute->back());
//                    subRoute->pop_back();
//                }
//            }
//
//            if (j < jUB)
//                tour[steps++] = findClosestChargingStation(back);
////            printf("---\n");
//            delete subRoute;
//        }
//
//        tour[steps++] = DEPOT;
//        i = j;
//    }
//    //check_solution(tour,steps);
//    for (int index = 0; index < steps; ++index) {
//        printf("%d, ",tour[index]);
//    }printf("\n");
//    double route_length = fitness_evaluation(tour, steps);
//    if (route_length < best_sol->tour_length) {
//        for (int index = 0; index < steps; ++index)
//            best_sol->tour[index] = tour[index];
//        best_sol->steps = steps;
//        best_sol->tour_length = route_length;
//    }
////    printf("END\n");
//    delete[] tour;
//    return route_length;
//}