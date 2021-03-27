//
// Created by wmw13 on 05/03/2021.
//

#include "GenerateTour.h"

/*
 * ================================================================================ *
 * GENERATE ROUTE
 * ================================================================================ *
 */
/*
 * Generates a route between charging stations and depot using the route of customers.
 * Returns a value determining the fitness of the inputted route.
 */
//double localSearch::getRouteLength(const int *route) {
//    /*
//    * GENERATE TOUR VERSION 1.
//    * Generates tour using a greedy approach.
//    * Provided with the competition framework.
//    */
//    int steps;
//    int *tour = new int[NUM_OF_CUSTOMERS + 1];
//    steps = 0;
//    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//        tour[index] = -1;
//    /*
//     * Sets the first item in the tour to DEPOT because all routes start at the depot.
//     * Increment steps to 1 due to first step was DEPOT.
//     */
//    tour[0] = DEPOT;
//    steps++;
//
//    int prev, next, chargingStation;
//    double activeCapacity = 0.0, activeBatteryLevel = 0.0;
//    int i = 0;
//    while (i <= NUM_OF_CUSTOMERS) {
//        prev = tour[steps - 1];
//        if (route[i] == 0) {
//            i++;
//            continue;
//        }
//        next = route[i];
//        if ((activeCapacity + get_customer_demand(next)) <= MAX_CAPACITY &&
//            activeBatteryLevel + get_energy_consumption(prev, next) <= BATTERY_CAPACITY) {
//            activeCapacity += get_customer_demand(next);
//            activeBatteryLevel += get_energy_consumption(prev, next);
//            tour[steps] = next;
//            steps++;
//            i++;
//        } else if ((activeCapacity + get_customer_demand(next)) > MAX_CAPACITY) {
//            activeCapacity = 0.0;
//            activeBatteryLevel = 0.0;
//            tour[steps] = DEPOT;
//            steps++;
//        } else if (activeBatteryLevel + get_energy_consumption(prev, next) > BATTERY_CAPACITY) {
////            chargingStation = rand() % (ACTUAL_PROBLEM_SIZE - NUM_OF_CUSTOMERS - 1) + NUM_OF_CUSTOMERS + 1;
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
//        } else {
//            activeCapacity = 0.0;
//            activeBatteryLevel = 0.0;
//            tour[steps] = DEPOT;
//            steps++;
//        }
//    }
//
//    //close EVRP tour to return back to the depot
//    if (tour[steps - 1] != DEPOT) {
//        tour[steps] = DEPOT;
//        steps++;
//    }
//    //check_solution(tour,steps);
//    double route_length = fitness_evaluation(tour, steps);
//    if (route_length < best_sol->tour_length) {
//        for (int index = 0; index < steps; ++index)
//            best_sol->tour[index] = tour[index];
//        best_sol->steps = steps;
//        best_sol->tour_length = route_length;
//    }
//    return route_length;
//}

/*
 * Generates a string which references the arc between A and B.
 */
std::string GenerateTour::getArcCode(int customerA, int customerB) {
    std::string output;
    std::string CustomerA = std::to_string(customerA);
    std::string CustomerB = std::to_string(customerB);
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
 * Finds the closest charging station to the inputted customer.
 */
int GenerateTour::findClosestChargingStation(int customer) {
    int chargingStation = NUM_OF_CUSTOMERS + NUM_OF_STATIONS;
    for (int index = NUM_OF_CUSTOMERS + 1; index <= (NUM_OF_CUSTOMERS + NUM_OF_STATIONS); index++) {
        if (get_distance(customer, index) < get_distance(customer, chargingStation) && is_charging_station(index))
            chargingStation = index;
    }
    return chargingStation;
}

/*
 * Gets the route's total distance, used to compare routes.
 */
double GenerateTour::getTotalDistance(int *route, int size) {
    double totalDist = 0.0;
    for (int i = 1; i < size; ++i)
        totalDist += get_distance(route[i - 1], route[i]);
    return totalDist;
}

/*
 * Runs through the route and determines whether the route satisfies the energy constraint.
 */
bool GenerateTour::getIsValidEnergy(int *route, int size) {
    bool isValid = true;
    double activeBattery = 0.0;

    for (int i = 1; i < size; ++i) {
        activeBattery += get_energy_consumption(route[i - 1], route[i]);
        if (activeBattery > BATTERY_CAPACITY) {
            isValid = false;
            break;
        }
        if (is_charging_station(route[i]))
            activeBattery = 0.0;
    }
    return isValid;
}

/*
 * Runs through the route and determines whether the route satisfies the load capacity constraint.
 */
bool GenerateTour::getIsValidCapacity(int *route, int size) {
    bool isValid = true;
    double activeCapacity = 0.0;

    for (int i = 0; i < size; ++i) {
        activeCapacity += get_customer_demand(route[i]);
        if (route[i] == DEPOT)
            activeCapacity = 0.0;
        if (activeCapacity > MAX_CAPACITY) {
            isValid = false;
            break;
        }
    }
    return isValid;
}

/*
 * Generates a route between charging stations and depot using the route of customers.
 * Returns a value determining the fitness of the inputted route.
 */
//double localSearch::getRouteLength(const int *routeA) {
///*
// * GENERATE TOUR VERSION 2.2
// * Uses a greedy approach to determine when to visit the depot.
// * Locates an optimal charging station location within the route.
// */
//    int route[NUM_OF_CUSTOMERS];
//    int ind = 0;
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
//        if(routeA[i] != DEPOT)
//            route[ind++] = routeA[i];
//    }
//
//
//    //Initialise variables.
//    int step = 0, front = 0, end = 0, tour[NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS];
//    for (int i = 0; i < NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS; ++i)
//        tour[i] = -1;
//
//    tour[step] = DEPOT;
//    step++;
//
//    while (front < NUM_OF_CUSTOMERS) {
//        double total = 0.0;
//        end = front;
//        int subRoute[NUM_OF_CUSTOMERS];
//        for (int i = 0; i < NUM_OF_CUSTOMERS; ++i)
//            subRoute[i] = -1;
//
//        int subRouteINDEX = 0;
//        subRoute[subRouteINDEX] = DEPOT;
//        ++subRouteINDEX;
//        for (int customers = front; customers < NUM_OF_CUSTOMERS; ++customers) {
//            if (route[customers] != 0) {
//                total += get_customer_demand(route[customers]);
//
//                if (total <= MAX_CAPACITY) {
//                    subRoute[subRouteINDEX] = route[customers];
//                    ++subRouteINDEX;
//                    end++;
//                }
//                else
//                    break;
//            }
//        }
//        subRoute[subRouteINDEX++] = DEPOT;
//
//        //Find optimal place for CS.
//        double minLength = INT_MAX;
//        int pos = -1, additions = 1, size = ((end - front));
//        bool isValidRoute = false, isValid;
//
//        size += 2;
//
//        if(!getIsValidEnergy(subRoute, subRouteINDEX)) {
//            while (!isValidRoute && additions > 0) {
//                additions = 0;
//                for (int index = 0; index < size - 1; ++index) {
//                    if ((is_charging_station(subRoute[index]) || is_charging_station(subRoute[index + 1])))
//                        continue;
//                    int subRouteWithCS[size + size], subRouteWithCSIndex = 0;
//                    for (int i = 0; i < size + size; ++i)
//                        subRouteWithCS[i] = -1;
//
//                    for (int i = 0; i < size; ++i) { //CHANGE
//                        subRouteWithCS[subRouteWithCSIndex] = subRoute[i];
//                        ++subRouteWithCSIndex;
//
//                        if (i == index) {
//                            subRouteWithCS[subRouteWithCSIndex] = findClosestChargingStation(subRoute[i]);
//                            ++subRouteWithCSIndex;
//                            ++additions;
//                        }
//
//                    }
//
//                    double tempDis = getTotalDistance(subRouteWithCS, subRouteWithCSIndex);
//                    isValid = getIsValidEnergy(subRouteWithCS, subRouteWithCSIndex);
//                    if (!isValidRoute && tempDis < minLength) {
//                        minLength = tempDis;
//                        pos = index;
//                        if (isValid)
//                            isValidRoute = isValid;
//                    } else if (isValidRoute && isValid && tempDis < minLength) {
//                        minLength = tempDis;
//                        pos = index;
//                    }
//                }
//                if (!isValidRoute) {
//                    int tempRoute[size + size];
//                    for (int i = 0; i < size + size; ++i)
//                        tempRoute[i] = -1;
//
//                    for (int i = 0; i < size; ++i) {
//                        tempRoute[i] = subRoute[i];
//                    }
//                    int subRouteIndex = 0;
//                    for (int i = 0; i < size; ++i) {
//                        subRoute[subRouteIndex] = tempRoute[i];
//                        ++subRouteIndex;
//                        if (i == pos) {
//                            subRoute[subRouteIndex] = findClosestChargingStation(tempRoute[i]);
//                            ++subRouteIndex;
//                        }
//
//                    }
//                    size = subRouteIndex;
//                    minLength = INT_MAX;
//                    pos = -1;
//                } else {
//                    int tempRoute[size + size];
//                    for (int i = 0; i < size + size; ++i)
//                        tempRoute[i] = -1;
//
//                    for (int i = 0; i < size; ++i) {
//                        tempRoute[i] = subRoute[i];
//                    }
//                    int subRouteIndex = 0;
//                    for (int i = 0; i < size; ++i) {
//                        subRoute[subRouteIndex++] = tempRoute[i];
//                        if (i == pos)
//                            subRoute[subRouteIndex++] = findClosestChargingStation(tempRoute[i]);
//
//                    }
//                }
//
//            }
//        }
//
//        for (int i = 0; i < size; ++i) {
////            if (subRoute[i] != DEPOT) {
//                tour[step] = subRoute[i];
//                ++step;
//            //}
//        }
//
//        tour[step++] = DEPOT;
//        front += (end-front);
//    }
//
//
//    double route_length = fitness_evaluation(tour, step);
//    if (route_length < best_sol->tour_length) {
//        //check_solution(tour,step);
//        for (int index = 0; index < step; ++index) {
//            int temp = tour[index];
//            best_sol->tour[index] = temp;
//        }
//        best_sol->steps = step;
//        best_sol->tour_length = route_length;
//    }
//    return route_length;
//
//}

/*
 * Gets the total amount of load to the next depot.
 */
double GenerateTour::getTotalLoadWithAddedDepot(int *route, int depotPos) {
    double total = 0.0;
    for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
        if (i == depotPos) {
            total += get_distance(route[i], DEPOT);
            if (i + 1 < NUM_OF_CUSTOMERS - 1)
                total += get_distance(DEPOT, route[i + 1]);
        } else {
            if (i + 1 < NUM_OF_CUSTOMERS - 1)
                total += get_distance(route[i], route[i + 1]);
        }
    }
    return total;
}

/*
 * Generates a route length quickly using a heuristic.
 */
double GenerateTour::getRouteLengthQuick(const int* route){
    double length = 0.0;
    double capacity = 0.0;
    double energy = 0.0;
    for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
        length += get_distance(route[i], route[i + 1]);
        capacity += get_customer_demand(route[i]);
        energy += get_energy_consumption(route[i],route[i+1]);
        if(capacity > MAX_CAPACITY){
            capacity = 0.0;
            energy = 0.0;
            length+= get_distance(route[i],DEPOT);
        }
    }

    return length;
}

/*
 * Generates a route between charging stations and depot using the route of customers.
 * Returns a value determining the fitness of the inputted route.
 */
double GenerateTour::getRouteLength(const int *routeA) {
/*
 * GENERATE TOUR VERSION 3
 * Uses a greedy approach to determine when to visit the depot.
 * Locates an optimal charging station location within the route.
 */

    int route[NUM_OF_CUSTOMERS];
    int ind = 0;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        if (routeA[i] != DEPOT)
            route[ind++] = routeA[i];
    }


    //Initialise variables.
    int step = 0, front = 0, end = 0, tour[NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS];
    for (int i = 0; i < NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS; ++i)
        tour[i] = -1;

    //Find Depot optimal placement.
//    int calcDepot[NUM_OF_CUSTOMERS + (int)(NUM_OF_CUSTOMERS*0.5)];
//    int calcDepotIndex;
//    //Set values to the current route.
//    for (int i = 0; i < NUM_OF_CUSTOMERS; ++i)
//        calcDepot[calcDepotIndex++] = route[i];

    bool loadValid = false;
    int lastDepot = 0;
    while (lastDepot < NUM_OF_CUSTOMERS) {
        int maxDepotPos = -1;
        double total = 0.0;
        for (int i = lastDepot; i < NUM_OF_CUSTOMERS; ++i) {
            total += get_customer_demand(route[i]);
            if (total > MAX_CAPACITY) {
                maxDepotPos = i - 1;
                break;
            }
        }
        //printf("MaxDepotPos : %d, LastDepot : %d\n",maxDepotPos,lastDepot);

        //Exit while loop when depot can't be found.
        if (maxDepotPos == -1)
            maxDepotPos = NUM_OF_CUSTOMERS - 1;

        //Depot goes after maxDepotPos.
        //Search starts at lastDepot (including lastDepot).

        double currentBestLength = INT_MAX, currentLength;
        int currentBestDepot = -1;
        int lowerBound = lastDepot;//maxDepotPos - (int)(((maxDepotPos-lastDepot)+1)*0.25);
        //Finds the optimal placement of the depot.
        for (int i = maxDepotPos; i >= lowerBound; --i) {
            //Gets the route length with the Depot at i.
            currentLength = getTotalLoadWithAddedDepot(route, i);
//            printf("bestLength = %f, currentLength = %f\n",currentBestLength,currentLength);
            if (currentLength < currentBestLength) {
                currentBestLength = currentLength;
                currentBestDepot = i;
            }
        }

//        printf("CurrentBest : %d, LastDepot : %d\n",currentBestDepot,lastDepot);
        //lastDepot to currentBestDepot is the sub-route for cs search (including currentBestDepot).
        int size = (currentBestDepot - lastDepot) + 1;
        int subRoute[size + size];
        int subRouteINDEX = 0;
        subRoute[subRouteINDEX++] = DEPOT;

        for (int i = lastDepot; i <= currentBestDepot; ++i) {
            subRoute[subRouteINDEX++] = route[i];
        }

        subRoute[subRouteINDEX++] = DEPOT;

//        for (int i = 0; i <subRouteINDEX ; ++i)
//            printf("*%d, ",subRoute[i]);
//        printf("\n");
        //Takes sub-route from depot to depot.
        //Find optimal place for CS.
        double minLength = INT_MAX;
        int pos = -1, additions = 1;
        bool isValidRoute = false, isValid;

        size += 2;

        if (!getIsValidEnergy(subRoute, subRouteINDEX)) {
            while (!isValidRoute && additions > 0) {
                additions = 0;
                for (int index = 0; index < size - 1; ++index) {
                    if ((is_charging_station(subRoute[index]) || is_charging_station(subRoute[index + 1])))
                        continue;
                    int subRouteWithCS[size + size], subRouteWithCSIndex = 0;
                    for (int i = 0; i < size + size; ++i)
                        subRouteWithCS[i] = -1;

                    for (int i = 0; i < size; ++i) { //CHANGE
                        subRouteWithCS[subRouteWithCSIndex] = subRoute[i];
                        ++subRouteWithCSIndex;

                        if (i == index) {
                            subRouteWithCS[subRouteWithCSIndex] = findClosestChargingStation(subRoute[i]);
                            ++subRouteWithCSIndex;
                            ++additions;
                        }

                    }

                    double tempDis = getTotalDistance(subRouteWithCS, subRouteWithCSIndex);
                    isValid = getIsValidEnergy(subRouteWithCS, subRouteWithCSIndex);
                    if (!isValidRoute && tempDis < minLength) {
                        minLength = tempDis;
                        pos = index;
                        if (isValid)
                            isValidRoute = isValid;
                    } else if (isValidRoute && isValid && tempDis < minLength) {
                        minLength = tempDis;
                        pos = index;
                    }
                }
                if (!isValidRoute) {
                    int tempRoute[size + size];
                    for (int i = 0; i < size + size; ++i)
                        tempRoute[i] = -1;

                    for (int i = 0; i < size; ++i) {
                        tempRoute[i] = subRoute[i];
                    }
                    int subRouteIndex = 0;
                    for (int i = 0; i < size; ++i) {
                        subRoute[subRouteIndex] = tempRoute[i];
                        ++subRouteIndex;
                        if (i == pos) {
                            subRoute[subRouteIndex] = findClosestChargingStation(tempRoute[i]);
                            ++subRouteIndex;
                        }

                    }
                    size = subRouteIndex;
                    minLength = INT_MAX;
                    pos = -1;
                } else {
                    int tempRoute[size + size];
                    for (int i = 0; i < size + size; ++i)
                        tempRoute[i] = -1;

                    for (int i = 0; i < size; ++i) {
                        tempRoute[i] = subRoute[i];
                    }
                    int subRouteIndex = 0;
                    for (int i = 0; i < size; ++i) {
                        subRoute[subRouteIndex++] = tempRoute[i];
                        if (i == pos)
                            subRoute[subRouteIndex++] = findClosestChargingStation(tempRoute[i]);

                    }
                }

            }
        }

//        for (int i = 0; i <size ; ++i)
//            printf("=%d, ",subRoute[i]);
//        printf("\n");
        for (int i = 0; i < size; ++i) {
//            if (subRoute[i] != DEPOT) {
            tour[step] = subRoute[i];
            ++step;
            //}
        }

        //CurrentBestDepot is the optimal depot placement.
        //Start searching for next depot from currentBestDepot+1;
        lastDepot = currentBestDepot + 1;
    }

    //MAY NEED CHANGING
    tour[step++] = DEPOT;
    //front += (end - front);

//   for (int i = 0; i <step ; ++i)
//        printf("%d, ",tour[i]);
//    printf("\n");
    if(checkSolution(tour,step)) {
        double route_length = fitness_evaluation(tour, step);
        if (route_length < best_sol->tour_length) {
//            printf("%f :: %f\n", best_sol->tour_length, route_length);
            check_solution(tour, step);

            //clean Route
            int counter = 0;
            for (int index = 0; index < step - 1; ++index) {
                int temp = tour[index];
                if (temp != tour[index + 1])
                    best_sol->tour[counter++] = temp;
            }
            best_sol->tour[counter++] = tour[step - 1];
            best_sol->steps = counter;
            best_sol->tour_length = route_length;
        }
        return route_length;
    }
    return INT_MAX;
}

/*
 * Checks that all the nodes are present within the tour.
 */
bool GenerateTour::checkAllCustomersVisited(int *tour, int size) {
    bool visited[NUM_OF_CUSTOMERS];
    for (int customer = 0; customer < NUM_OF_CUSTOMERS; ++customer) {
        visited[customer] = false;
    }
    for (int customer = 0; customer < size; ++customer) {
        if (!is_charging_station(tour[customer])) {
            visited[tour[customer] - 1] = true;
        }
    }
    bool isValid = true;
    for (int customer = 0; customer < NUM_OF_CUSTOMERS; ++customer) {
        if (!visited[customer])
            isValid = false;
    }
    return isValid;
}

/*
 * Checks that the solution is a valid solution.
 */
bool GenerateTour::checkSolution(int *tour, int size) {
    bool energyValid = getIsValidEnergy(tour, size);
    bool capacityValid = getIsValidCapacity(tour, size);
    bool allCustomersVisited = checkAllCustomersVisited(tour, size);

    return energyValid && capacityValid && allCustomersVisited;
}

/*
 * Gets a route length quickly without generating a route with charging stations and depots.
 */
double GenerateTour::getBasicLength(int *bestRoute) {
    double routeLength = 0.0;
    for (int i = 1; i <= NUM_OF_CUSTOMERS; ++i)
        routeLength += get_distance(bestRoute[i - 1], bestRoute[i]);
    return routeLength;
}


/*
 * ================================================================================ *
 * OLD CODE
 * ================================================================================ *
 */

//double localSearch::getRouteLength(const int *route) {
///*
// * GENERATE TOUR VERSION 2
// */
//    int *Routes = new int[NUM_OF_CUSTOMERS];
//    int *tour = new int[NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS];
//    int tempIndex = 0, step = 0;
//    //INITALISE ARRAYS.
//    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index) {
//        if (route[index] != 0)
//            Routes[tempIndex++] = route[index];
//    }
////    for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
////        printf("%d, ",tempRoute[i]);
////    }printf("\n");
//    for (int index = 0; index < NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS; ++index) {
//        tour[index] = -1;
//    }
//    tour[step++] = DEPOT;
////    printf("start\n");
//
//    int start = 0;
//    int maxDistForCap;
//    int csPos;
//    while (start < NUM_OF_CUSTOMERS) {
////        maxDistForCap = getTotalLoad(Routes, start);
//        double total = 0.0;
//        maxDistForCap = 0;
//        //get total load.
//        for (int customers = start; customers <= NUM_OF_CUSTOMERS; ++customers) {
//            if (route[customers] != 0) {
//                total += get_customer_demand(route[customers]);
//                if (total <= MAX_CAPACITY)
//                    maxDistForCap++;
//                else
//                    break;
//            }
//        }
////        double totalEnergy = getTotalEnergyConsumption(Routes, start, start + maxDistForCap);
//        double totalEnergy = 0.0;
//        //Get Total energy.
//        totalEnergy += get_energy_consumption(DEPOT, route[start]);
//        totalEnergy += get_energy_consumption(route[start + maxDistForCap - 1], DEPOT);
//        for (int customers = start + 1; customers < start + maxDistForCap; ++customers) {
//            totalEnergy += get_energy_consumption(route[customers - 1], route[customers]);
//        }
////        int space = (maxDistForCap)/numberOfStations;
////        int dif = 1;
//        if (totalEnergy < BATTERY_CAPACITY) {
////            printf("WITHIN CAP\n");
//            for (int i = start; i < start + maxDistForCap && i < NUM_OF_CUSTOMERS; ++i) {
//                tour[step++] = Routes[i];
////                printf("%d, ",tempRoute[i]);
//            }
////            printf("\n");
//
//        } else {
//            /*
//            * FIND OPTIMAL PATH WITH CHARGING STATIONS.
//            */
//            int *tempTempRoute = new int[maxDistForCap + 2];
//            tempTempRoute[0] = DEPOT;
//            int index = 1;
////            for (int i = start; i <start + maxDistForCap; ++i) {
////                printf("%d, ",Routes[i]);
////            }printf("--\n");
//            for (int i = start; i < start + maxDistForCap; ++i)
//                if(Routes[i] != -1)
//                    tempTempRoute[index++] = Routes[i];
//            tempTempRoute[index++] = DEPOT;
//            tempTempRoute = findOptimalCS(tempTempRoute, index);
//            for (int i = 1; i < tempTempRoute[0]; ++i) {
//                if (tempTempRoute[i] != DEPOT)
//                    tour[step++] = tempTempRoute[i];
//            }
//            delete[] tempTempRoute;
//        }
//        tour[step++] = DEPOT;
//        start += maxDistForCap;
//
//    }
//
////    for (int i = 0; i < step; ++i) {
////        printf("%d, ", tour[i]);
////    }
////    printf("\n");
////    check_solution(tour, step);
//    double route_length = fitness_evaluation(tour, step);
//    if (route_length < best_sol->tour_length) {
//        for (int index = 0; index < step; ++index) {
//            int temp = tour[index];
//            best_sol->tour[index] = temp;
//        }
//        best_sol->steps = step;
//        best_sol->tour_length = route_length;
//    }
//    delete[] Routes;
//    delete[] tour;
//    return route_length;
//
//}

//int *localSearch::findOptimalCS(int *route, int size) {
//
//    int *subRoute = new int[size + size];
//    for (int i = 0; i < size; ++i) {
//        subRoute[i] = route[i];
////        printf("%d, ", route[i]);
//
//    }    //printf("\n");
//
//    for (int i = size; i < size + size; ++i) {
//        subRoute[i] = -1;
//    }
//
//    double minLength = INT_MAX;
//    int numberOfStation = size, pos = -1, additions = 1;
//    bool isValidRoute = false, isValid;
//    while (!isValidRoute && additions > 0) {
//        additions = 0;
////        for (int i = 0; i < size; ++i) {
////            printf("%d, ", subRoute[i]);
////        }
////        printf("\n");
//        for (int index = 0; index < size - 1; ++index) {
//            if ((is_charging_station(subRoute[index]) || is_charging_station(subRoute[index + 1])))
//                continue;
//            int *subRouteWithCS = new int[size + numberOfStation], subRouteWithCSIndex = 0;
//            for (int i = 0; i < size; ++i) { //CHANGE
//                subRouteWithCS[subRouteWithCSIndex++] = subRoute[i];
//                if (i == index) {
//                    subRouteWithCS[subRouteWithCSIndex++] = findClosestChargingStation(subRoute[i]);
//                    additions++;
//                }
//            }
//            double tempDis = getTotalDistance(subRouteWithCS, subRouteWithCSIndex);
//            isValid = getIsValidEnergy(subRouteWithCS, subRouteWithCSIndex);
//            if (!isValidRoute && tempDis < minLength) {
//                minLength = tempDis;
//                pos = index;
//                if (isValid)
//                    isValidRoute = isValid;
//            } else if (isValidRoute && isValid && tempDis < minLength) {
//                minLength = tempDis;
//                pos = index;
//            }
//            delete[]subRouteWithCS;
//        }
//        if (!isValidRoute) {
//            int *tempRoute = new int[size + size];
//            for (int i = 0; i < size; ++i) {
//                tempRoute[i] = subRoute[i];
//            }
//            int subRouteIndex = 0;
//            for (int i = 0; i < size; ++i) {
//                subRoute[subRouteIndex++] = tempRoute[i];
////                printf("<%d\n",tempRoute[i]);
//                if (i == pos)
//                    subRoute[subRouteIndex++] = findClosestChargingStation(tempRoute[i]);
//
//            }
//            delete[] tempRoute;
//            size = subRouteIndex;
////            printf("POS %d\n",pos);
////            for (int i = 0; i < subRouteIndex; ++i) {
////                printf("%d, ", subRoute[i]);
////            }
////            printf("\n");
//            minLength = INT_MAX;
////            isValidRoute = false;
//            numberOfStation = size;
//            pos = -1;
//        }
//    }
//
////    if (isValidRoute || additions == 0) {
//
//    int *output = new int[size + numberOfStation + 1], outputIndex = 1;
//
//    for (int i = 0; i < size; ++i) {
//        output[outputIndex++] = subRoute[i];
//        if (i == pos)
//            output[outputIndex++] = findClosestChargingStation(subRoute[i]);
//    }
////    printf("POS %d\n",pos);
////    for (int i = 1; i < outputIndex; ++i) {
////        printf("%d, ", output[i]);
////    }
////    printf("\n");
//    output[0] = outputIndex;
////    delete[] route;
//    delete[] subRoute;
//    return output;
////    } else {
////        int *output = new int[size + numberOfStation], outputIndex = 0;
////        for (int i = 0; i < size; ++i) {
////            output[outputIndex++] = subRoute[i];
////            if (i == pos)
////                output[outputIndex++] = findClosestChargingStation(subRoute[i]);
////        }
//////        for (int i = 0; i < outputIndex; ++i) {
//////            printf("%d, ", output[i]);
//////        }
//////        printf("\n");
//////        printf("%d\n", getIsValidEnergy(output, outputIndex));
////        delete[] subRoute;
////        return findOptimalCS(output,outputIndex);
////    }
//}
//
//int localSearch::getTotalLoad(int *route, int startCustomer) {
//    double total = 0.0;
//    int size = 0;
//
//    for (int customers = startCustomer; customers <= NUM_OF_CUSTOMERS; ++customers) {
//        if (route[customers] != 0) {
//            total += get_customer_demand(route[customers]);
//            if (total <= MAX_CAPACITY)
//                size++;
//            else
//                break;
//        }
//    }
//    return size;
//}
//
//double localSearch::getTotalEnergyConsumption(int *route, int startCustomer, int upperBound) {
//    double total = 0.0;
//    total += get_energy_consumption(DEPOT, route[startCustomer]);
//    total += get_energy_consumption(route[upperBound - 1], DEPOT);
//    for (int customers = startCustomer + 1; customers < upperBound; ++customers) {
//        total += get_energy_consumption(route[customers - 1], route[customers]);
//    }
//    return total;
//}

//bool localSearch::getIsValidEnergy(int *route, int size) {
////    printf("Max Battery %d\n",BATTERY_CAPACITY);
//    bool isValid = true;
//    double activeBattery = 0.0;
////    for (int i = 0; i <size ; ++i) {
////        printf("%d, ",route[i]);
////    }printf("\n");
//    for (int i = 1; i < size; ++i) {
//        activeBattery += get_energy_consumption(route[i - 1], route[i]);
////        printf("checking %d -- %d energy %f, current load %f\n",route[i - 1],route[i],get_energy_consumption(route[i - 1], route[i]),activeBattery);
//        if (activeBattery > BATTERY_CAPACITY) {
////            printf("FALSE\n");
//            isValid = false;
//            break;
//        }
//        if (is_charging_station(route[i]))
//            activeBattery = 0.0;
//    }
//
//    return isValid;
//}

//double localSearch::getRouteLength(const int *routeA) {
///*
// * GENERATE TOUR VERSION 2.1
// */
//
//    int route[NUM_OF_CUSTOMERS];
//    int ind = 0;
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
//        if(routeA[i] != DEPOT)
//            route[ind++] = routeA[i];
//    }
//
////    printf("ROUTE:  ");
////    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
////        printf("%d, ",route[i]);
////    }printf("\n");
//
//    //Initialise variables.
//    int step = 0, front = 0, end = 0, tour[NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS];
//    for (int i = 0; i < NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS; ++i)
//        tour[i] = -1;
//
//    //Set start to DEPOT
//    tour[step] = DEPOT;
//    step++;
//    //front++;
//
//    while (front < NUM_OF_CUSTOMERS-1) {
//        double total = 0.0;
//        end = front;
//        //get total load. Finds Front to End pointers is the entire route for one load.
////        printf("MAX CAP:: %d  ",MAX_CAPACITY);
//        for (int customers = front; customers <= NUM_OF_CUSTOMERS; ++customers) {
//            if (route[customers] != 0) {
//                total += get_customer_demand(route[customers]);
//
//                if (total <= MAX_CAPACITY) {
////                    printf("^%d::%f, ", route[customers],total);
//                    end++;}
//                else
//                    break;
//            }
//        }
////        printf("\n");
//
////        for (int i = front; i < end; ++i) {
////            printf("%d, ", route[i]);
////        }printf("\n");
//        if(end >= NUM_OF_CUSTOMERS)
//            end=NUM_OF_CUSTOMERS-1;
////        printf("\n");
////        printf("f %d e %d\n",front,end);
//
//        /*
//         *          F         E
//         *          |         |
//         * |0|1|2|3|4|5|6|7|8|9|10|11|12|13|
//         *
//         * start at 4, loop up to 9 i=4; i<=9; i++
//         */
//
//        //Find optimal place for CS.
//        double minLength = INT_MAX;
//        int pos = -1, additions = 1, size = ((end - front));
//        bool isValidRoute = false, isValid;
//
//        int subRoute[size + size];
//        for (int i = 0; i < size + size; ++i)
//            subRoute[i] = -1;
//
//        subRoute[0] = DEPOT;
//        int routeIndex = front;
//        for (int i = 1; i < size + 1; ++i) {
////            printf("ri %d ",routeIndex);
//            subRoute[i] = route[routeIndex];
//            ++routeIndex;
////           printf("%d, ",subRoute[i]);
//        }
////        printf("\n");
//        subRoute[size + 1] = DEPOT;
//        size += 2;
//
//        while (!isValidRoute && additions > 0) {
//            additions = 0;
//            for (int index = 0; index < size - 1; ++index) {
//                if ((is_charging_station(subRoute[index]) || is_charging_station(subRoute[index + 1])))
//                    continue;
//                int subRouteWithCS[size + size], subRouteWithCSIndex = 0;
//                for (int i = 0; i < size + size; ++i)
//                    subRouteWithCS[i] = -1;
//
//                for (int i = 0; i < size; ++i) { //CHANGE
//                    subRouteWithCS[subRouteWithCSIndex] = subRoute[i];
//                    ++subRouteWithCSIndex;
//
//                    if (i == index) {
////                        printf("%d ++ %d\n", subRoute[i],findClosestChargingStation(subRoute[i]));
//                        subRouteWithCS[subRouteWithCSIndex] = findClosestChargingStation(subRoute[i]);
//                        ++subRouteWithCSIndex;
//                        ++additions;
//                    }
//
//                }
////                for (int i = 0; i < subRouteWithCSIndex; ++i) {
////                    printf("=%d, ", subRouteWithCS[i]);
////                }
////                printf("\n");
//
//                double tempDis = getTotalDistance(subRouteWithCS, subRouteWithCSIndex);
//                isValid = getIsValidEnergy(subRouteWithCS, subRouteWithCSIndex);
////                printf("dis %f=%f -- %d\n",tempDis,minLength,isValid);
//                if (!isValidRoute && tempDis < minLength) {
//                    minLength = tempDis;
//                    pos = index;
//                    if (isValid)
//                        isValidRoute = isValid;
//                } else if (isValidRoute && isValid && tempDis < minLength) {
//                    minLength = tempDis;
//                    pos = index;
//                }
//            }
//            if (!isValidRoute) {
//                int tempRoute[size + size];
//                for (int i = 0; i < size + size; ++i)
//                    tempRoute[i] = -1;
//
//                for (int i = 0; i < size; ++i) {
//                    tempRoute[i] = subRoute[i];
//                }
//                int subRouteIndex = 0;
//                for (int i = 0; i < size; ++i) {
//                    subRoute[subRouteIndex] = tempRoute[i];
//                    ++subRouteIndex;
//                    if (i == pos) {
//                        subRoute[subRouteIndex] = findClosestChargingStation(tempRoute[i]);
//                        ++subRouteIndex;
//                    }
//
//                }
//                size = subRouteIndex;
//                minLength = INT_MAX;
//                pos = -1;
//            }
//            else{
//                int tempRoute[size + size];
//                for (int i = 0; i < size + size; ++i)
//                    tempRoute[i] = -1;
//
//                for (int i = 0; i < size; ++i) {
//                    tempRoute[i] = subRoute[i];
//                }
//                int subRouteIndex = 0;
//                for (int i = 0; i < size; ++i) {
//                    subRoute[subRouteIndex++] = tempRoute[i];
//                    if (i == pos)
//                        subRoute[subRouteIndex++] = findClosestChargingStation(tempRoute[i]);
//
//                }
//            }
//
//        }
////        printf("Pos %d\n",pos); //where is the adding to the tour for 1 CS
////        if(pos==-1){ printf("minus pos\n");}
//        for (int i = 0; i < size; ++i) {
//            if (subRoute[i] != DEPOT) {
//                tour[step] = subRoute[i];
////                printf("*%d, ", tour[step]);
//                ++step;
//            }
//        }
//
//        tour[step++] = DEPOT;
////        printf("%d, ",tour[step-1]);
////         printf("\n");
////        printf("--f %d e %d\n",front,end);
//
////        for (int i = 0; i <step; ++i) {
////            printf("$%d, ",tour[i]);
////        }printf("\n");
//
//        front += (end-front);
////        printf("-=f %d e %d\n",front,end);
//
//
//    }
////    printf("=======================================\n");
//
////    for (int i = 0; i <step; ++i) {
////        printf("%d, ",tour[i]);
////    }printf("\n");
//
//    double route_length = fitness_evaluation(tour, step);
////    printf("%f -- %f\n",route_length, best_sol->tour_length);
//    if (route_length < best_sol->tour_length) {
//
//        check_solution(tour,step);
//        for (int index = 0; index < step; ++index) {
//            int temp = tour[index];
//            best_sol->tour[index] = temp;
//        }
//        best_sol->steps = step;
//        best_sol->tour_length = route_length;
//    }
//    return route_length;
//
//}

//double localSearch::getRouteLength(const int *routeA) {
//    /*
//     * GENERATE TOUR VERSION 3
//     */
//    //Removes the depot from the input route.
//    int route[NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS];
//    int ind = 0;
//    route[ind++] = DEPOT;
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
//        if(routeA[i] != DEPOT)
//            route[ind++] = routeA[i];
//    }
//    route[ind++] = DEPOT;
//    int size = ind;
//
//    int current = -1, next = -1, pos = -1, step = 0, additions = 1,tour[NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS];
//    for (int i = 0; i < NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS; ++i)
//        tour[i] = -1;
//    bool isValidRoute = false, isValid = false;
//    double totalEnergy = 0.0;
//    double minLength = INT_MAX;
//
//    //STAGE 1: locate CS for whole route.
//    while (!isValidRoute && additions > 0) {
//        additions = 0;
//        for (int i = 0; i < NUM_OF_CUSTOMERS+1; ++i) {
//            if ((is_charging_station(route[i]) || is_charging_station(route[i + 1])))
//                continue;
//
//            //Initialise temp array routeCalc.
//            int routeCalcIndex = 0, routeCalc[NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS];
//            for (int j = 0; j < NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS; ++j) {
//                routeCalc[j] = -1;
//            }
//
//            //Find placement of CS.
//            for (int j = 0; j < NUM_OF_CUSTOMERS+2; ++j) {
//                routeCalc[routeCalcIndex] = route[j];
//                ++routeCalcIndex;
//                if (j == i) {
//                    routeCalc[routeCalcIndex] = findClosestChargingStation(route[j]);
//                    ++routeCalcIndex;
//                    ++additions;
//                }
//            }
//
//            //Check whether placement of CS is valid.
//            double tempDis = getTotalDistance(routeCalc, routeCalcIndex);
//            isValid = getIsValidEnergy(routeCalc, routeCalcIndex);
//            if (!isValidRoute && tempDis < minLength) {
//                minLength = tempDis;
//                pos = i;
//                if (isValid)
//                    isValidRoute = isValid;
//            } else if (isValidRoute && isValid && tempDis < minLength) {
//                minLength = tempDis;
//                pos = i;
//            }
//        }
//
//        //If route with CS is not valid prepare array for while loop iteration.
//        if (!isValidRoute) {
//            int tempRoute[size + size];
//            for (int i = 0; i < size + size; ++i)
//                tempRoute[i] = -1;
//            for (int i = 0; i < size; ++i) {
//                tempRoute[i] = route[i];
//            }
//            int routeIndex = 0;
//            for (int i = 0; i < size; ++i) {
//                route[routeIndex] = tempRoute[i];
//                ++routeIndex;
//                if (i == pos) {
//                    route[routeIndex] = findClosestChargingStation(tempRoute[i]);
//                    ++routeIndex;
//                }
//
//            }
//            size = routeIndex;
//            minLength = INT_MAX;
//            pos = -1;
//        }
//            //Route with CS is valid, move to next stage.
//        else {
//            int tempRoute[size + size];
//            for (int i = 0; i < size + size; ++i)
//                tempRoute[i] = -1;
//
//            for (int i = 0; i < size; ++i) {
//                tempRoute[i] = route[i];
//            }
//            int routeIndex = 0;
//            for (int i = 0; i < size; ++i) {
//                route[routeIndex++] = tempRoute[i];
//                if (i == pos)
//                    route[routeIndex++] = findClosestChargingStation(tempRoute[i]);
//
//            }
//        }
//    }
//
//
//
//    //STAGE 2: Find depot placement.
//    isValidRoute = false; isValid = false; additions = 1;
//
////    while (!isValidRoute && additions > 0) {
////        additions = 0;
////        for (int i = 0; i < NUM_OF_CUSTOMERS+1; ++i) {
////            //Skip if current or next is the DEPOT.
////            if (route[i] == DEPOT || route[i + 1] == DEPOT)
////                continue;
////
////            //Initialise temp array routeCalc.
////            int routeCalcIndex = 0, routeCalc[NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS];
////            for (int j = 0; j < NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS; ++j) {
////                routeCalc[j] = -1;
////            }
////
////            //Find placement of DEPOTs.
////            for (int j = 0; j < NUM_OF_CUSTOMERS+2; ++j) {
////                routeCalc[routeCalcIndex] = route[j];
////                ++routeCalcIndex;
////                if (j == i) {
////                    routeCalc[routeCalcIndex] = DEPOT;
////                    ++routeCalcIndex;
////                    ++additions;
////                }
////            }
////
////            //Check whether placement of CS is valid.
////            double tempDis = getTotalDistance(routeCalc, routeCalcIndex);
////            isValid = getIsValidCapacity(routeCalc, routeCalcIndex);
////            if (!isValidRoute && tempDis < minLength) {
////                minLength = tempDis;
////                pos = i;
////                if (isValid)
////                    isValidRoute = isValid;
////            } else if (isValidRoute && isValid && tempDis < minLength) {
////                minLength = tempDis;
////                pos = i;
////            }
////        }
////
////        //If route with CS is not valid prepare array for while loop iteration.
////        if (!isValidRoute) {
////            int tempRoute[size + size];
////            for (int i = 0; i < size + size; ++i)
////                tempRoute[i] = -1;
////            for (int i = 0; i < size; ++i) {
////                tempRoute[i] = route[i];
////            }
////            int routeIndex = 0;
////            for (int i = 0; i < size; ++i) {
////                route[routeIndex] = tempRoute[i];
////                ++routeIndex;
////                if (i == pos) {
////                    route[routeIndex] = DEPOT;
////                    ++routeIndex;
////                }
////
////            }
////            size = routeIndex;
////            minLength = INT_MAX;
////            pos = -1;
////        }
////            //Route with CS is valid, move to next stage.
////        else {
////            int tempRoute[size + size];
////            for (int i = 0; i < size + size; ++i)
////                tempRoute[i] = -1;
////
////            for (int i = 0; i < size; ++i) {
////                tempRoute[i] = route[i];
////            }
////            int routeIndex = 0;
////            for (int i = 0; i < size; ++i) {
////                route[routeIndex++] = tempRoute[i];
////                if (i == pos)
////                    route[routeIndex++] = DEPOT;
////
////            }
////        }
////    }
//    double totalCapacity = 0.0;
//    int tempRoute[size];
//    for (int i = 0; i < size; ++i) {
//        tempRoute[i] = route[i];
//    }
//    int routeIndex = 0;
//    for (int i = 0; i < size; ++i) {
//        if(tempRoute[i] == DEPOT)
//            totalCapacity = 0.0;
//        else{
//            totalCapacity += get_customer_demand(tempRoute[i]);
//            if (totalCapacity >= MAX_CAPACITY*0.98){
//                totalCapacity = 0.0;
//                route[routeIndex++] = DEPOT;
//            }
//        }
//        route[routeIndex++] = tempRoute[i];
//    }
//
//    //STAGE 3: Remove unnecessary CS within the route.
//
//
//
//    //STAGE 4: Set the final route found to the tour for evaluation.
//    for (int i = 0; i < size; ++i)
//        tour[i] = route[i];
//
////    for (int i = 0; i < size; ++i)
////        printf("%d, ",tour[i]);
////    printf("\n");
//
//    //STAGE 5: Check solution is valid. If solution is better than current best set it to current best.
//    double route_length = fitness_evaluation(tour, size);
//    // printf("rl %f\n",route_length);
//    if (route_length < best_sol->tour_length) {
//
//        //check_solution(tour,size);
//        for (int index = 0; index < size; ++index) {
//            int temp = tour[index];
//            best_sol->tour[index] = temp;
//        }
//        best_sol->steps = size;
//        best_sol->tour_length = route_length;
//    }
//    return route_length;
//}