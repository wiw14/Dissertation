//
// Created by wmw13 on 15/02/2021.
//

#include "KNN.h"


using namespace std;

KNN::KNN(int KNN) {
    LS = new localSearch(3,3);
    v = new int[NUM_OF_CUSTOMERS+1];
    filter = new int[NUM_OF_CUSTOMERS+1];
    neighbours = new int*[NUM_OF_CUSTOMERS+1];
//    randomSearchIteration = 3;
//    twoOptIterations = 3;

    for (int index = 0; index <= NUM_OF_CUSTOMERS ; ++index) {
        neighbours[index] = new int[KNN];
        filter[index] = 0;
        v[index] = -1;
    }
    for (int customer = 0; customer <= NUM_OF_CUSTOMERS; ++customer) {
        neighbours[customer] = getKNN(customer,KNN);
    }
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
//        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
//            localSearchPheromone[getArcCode(i,j)] = 1;
//        }
//    }
    generateRoute(KNN);
    LS->randomPheromoneLocalSearchWithTwoOpt(v);
    //LS->randomPheromoneLocalSearch(v);
    //LS->randomLocalSearch(v);
    //LS->twoOptLocalSearch(v);
    LS->getRouteLength(v);
    int* vReverse = new int[NUM_OF_CUSTOMERS+1];
    int tempIndex = NUM_OF_CUSTOMERS;
    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index)
        vReverse[index]=v[tempIndex--];
    LS->getRouteLength(vReverse);
    delete[]vReverse;
}

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

KNN::~KNN(){
    for (int index = 0; index <= NUM_OF_CUSTOMERS ; ++index)
        delete[] neighbours[index];
    delete[] v;
    delete LS;
}

void KNN::generateRoute(int KNN){
    int index;
    int next;
    v[0] = 0;
    filter[0] = 1;
    for (int customer = 1; customer <= NUM_OF_CUSTOMERS+1; ++customer) {
        //printf("%d\n",v[customer-1]);
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
int KNN::findNextClosestNode(int customer){
    //printf("--%d--",customer);
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

void KNN::printRoute(){
    for (int customer = 0; customer <=NUM_OF_CUSTOMERS; ++customer)
        printf("%d, ", v[customer]);
    printf("\n");
}
//
//double KNN::getRouteLength(const int *route) {
//
//    int steps;
//    int *tour = new int[NUM_OF_CUSTOMERS + 1];
//    /*
//    * Re-Initialise best_sol
//    */
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
//
//    double route_length = fitness_evaluation(tour,steps);
//    if (route_length < best_sol->tour_length){
//        for (int index = 0; index < steps; ++index)
//            best_sol->tour[index] = tour[index];
//        best_sol->steps = steps;
//        best_sol->tour_length = route_length;
//    }
//    return route_length;
//}
//
///*
// * 2-opt local search.
// */
//void KNN::twoOptLocalSearch() {
//    int improve = 0;
//    int* tempRoute = new int[NUM_OF_CUSTOMERS+1];
//    while ( improve < twoOptIterations ){
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            tempRoute[index] = v[index];
//        double route_length = getRouteLength(v);
//        for (int i = 0; i < NUM_OF_CUSTOMERS; ++i){
//            for (int j = i + 1; j <= NUM_OF_CUSTOMERS ; ++j) {
//                //printf("i %d, j %d\n",i,j);
//                twoOptSwap(i,j,tempRoute,v);
//                double new_route_length = getRouteLength(tempRoute);
//                if (new_route_length < route_length){
//                    improve = 0;
//                    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//                        v[index] = tempRoute[index];
//                    route_length = new_route_length;
//                }
//            }
//        }
//        improve++;
//    }
//    delete[] tempRoute;
////    for (int i = 0; i <= NUM_OF_CUSTOMERS ; ++i)
////        printf("%d, ",v[i]);
////    printf("\n");
//}
//
///*
// * Swaps the route between the inputted points used in 2-opt local search.
// */
//void KNN::twoOptSwap(int i , int j, int* route, const int* currRoute){
//    for (int k = 0; k < i; ++k)
//        route[k] = currRoute[k];
//    int index = i;
//    for (int k = j; k >= i; --k) {
//        route[index] = currRoute[k];
//        index++;
//    }
//    for (int k = j+1; k <= NUM_OF_CUSTOMERS; ++k) {
//        route[k] = currRoute[k];
//    }
//}
//
///*
// * 2-opt local search modified to work alongside Pheromone Random Local Search.
// */
//void KNN::twoOptLocalPheromoneAddonSearch(int* currentRoute) {
//    int improve = 0;
//    int* tempRoute = new int[NUM_OF_CUSTOMERS+1];
//    while ( improve < twoOptIterations ){
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            tempRoute[index] = currentRoute[index];
//        double route_length = getRouteLength(currentRoute);
//        for (int i = 0; i < NUM_OF_CUSTOMERS; ++i){
//            for (int j = i + 1; j <= NUM_OF_CUSTOMERS ; ++j) {
//                //printf("i %d, j %d\n",i,j);
//                twoOptSwap(i,j,tempRoute,currentRoute);
//                double new_route_length = getRouteLength(tempRoute);
//                if (new_route_length < route_length){
//                    improve = 0;
//                    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//                        currentRoute[index] = tempRoute[index];
//                    route_length = new_route_length;
//                }
//            }
//        }
//        improve++;
//    }
//    delete[] tempRoute;
////    for (int i = 0; i <= NUM_OF_CUSTOMERS ; ++i)
////        printf("%d, ",v[i]);
////    printf("\n");
//}
//
///*
// * Decreases all the pheromones being used for the weighted random number generator used in the local search.
// */
//void KNN::decreaseLocalSearchPheromone() {
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
//        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
//            if(localSearchPheromone[getArcCode(i,j)] > 1)
//                localSearchPheromone[getArcCode(i,j)] = localSearchPheromone[getArcCode(i,j)] * 0.8;
//        }
//    }
//}
//
///*
// * Gets the total number of pheromones in the system, used for weighted random number generator.
// */
//int KNN::getTotalWeight() {
//    int totalWeight = 0;
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
//        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
//            totalWeight += localSearchPheromone[getArcCode(i, j)];
//        }
//    }
//    return totalWeight;
//}
//
///*
// * Generates a random number pair (x and y) based on the local search pheromones.
// */
//std::vector<int> KNN::getRandomNumber() { //type either x (0) or y (1).
//    std::vector<int> xy(2);
//    int totalWeight = getTotalWeight();
//    xy.operator[](0) = -1; xy.operator[](1) = -1;
//    //printf("type: %d tw: %d\n",type ,totalWeight); //DEBUGGING
//    int val = rand() % (totalWeight - 1);
//    totalWeight = 0;
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
//        for (int j = i + 1; j <= NUM_OF_CUSTOMERS; j++) {
//            totalWeight += localSearchPheromone[getArcCode(i,j)];
//            if (val <= (totalWeight)) {
//                //printf("%d\n",index); //DEBUGGING
//                xy.operator[](0) = i;
//                xy.operator[](1) = j;
//                return xy;
//            }
//        }
//    }
//    return xy;
//}
//
///*
// * Randomly switches two customers in the route, to find an optimal solution.
// */
//void KNN::randomLocalSearch() {
//    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
//    double route_length = getRouteLength(v);
//    double new_route_length = route_length;
//    int iters = 0, x, y;
//    while (new_route_length >= route_length && iters < randomSearchIteration) {
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            tempRoute[index] = v[index];
//        iters++;
//        x = rand() % NUM_OF_CUSTOMERS;
//        y = ((rand() % 5) + 1 + x);
//        if (y > NUM_OF_CUSTOMERS) {
//            y = NUM_OF_CUSTOMERS;
//        }
//        //printf("X = %d, Y = %d\n", x, y); //DEBUGGING
//
//        int size = (y - x) + 1;
//        int *routeTemp = new int[size];
//        for (int index = 0; index < size; index++)
//            routeTemp[index] = -1;
//        int tempIndex = 0;
//
//        for (int index = y; index >= x; index--) {
//            routeTemp[tempIndex] = tempRoute[index];
//            tempIndex++;
//        }
//        tempIndex = 0;
//        for (int index = x; index <= y; index++) {
//            tempRoute[index] = routeTemp[tempIndex];
//            tempIndex++;
//        }
//        delete[] routeTemp;
//        new_route_length = getRouteLength(tempRoute);
//
//        //DEBUGGING
////        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
////            printf("%d, ",tempRoute[index]);
////        printf("\n");
//    }
//    //Two Opt Local As Well
//    //ACO::twoOptLocalPheromoneAddonSearch(tempRoute);
//    if (new_route_length < route_length) {
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            v[index] = tempRoute[index];
//    }
//    delete[] tempRoute;
//}
//
///*
// * Switches two customers based on a weighted average determined by pheromones.
// */
//void KNN::randomPheromoneLocalSearch() {
//    int *tempRoute = new int[NUM_OF_CUSTOMERS + 1];
//    double route_length = KNN::getRouteLength(v);
//    double new_route_length = route_length;
//    int iters = 0, x, y;
//    std::vector<int> xy(2);
//    while (new_route_length >= route_length && iters < randomSearchIteration) { //(rand()%5)+10)
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            tempRoute[index] = v[index];
//        iters++;
//        //x = rand() % NUM_OF_CUSTOMERS;
//        //y = ((rand() % 5) + 1 + x);
//
//        xy = getRandomNumber();
//        if (xy.operator[](0) < xy.operator[](1)){
//            x = xy.operator[](0);
//            y = xy.operator[](1);
//        }
//        else{
//            x = xy.operator[](1);
//            y = xy.operator[](0);
//        }
//
////        y = getRandomNumber(1);
////        int tempx;
////        if (y < x) {
////            tempx = x;
////            x = y;
////            y = tempx;
////        }
//        //printf("X = %d, Y = %d\n", x, y); //DEBUGGING
//
//        int size = (y - x) + 1;
//        int *routeTemp = new int[size];
//        for (int index = 0; index < size; index++)
//            routeTemp[index] = -1;
//        int tempIndex = 0;
//
//        for (int index = y; index >= x; index--) {
//            routeTemp[tempIndex] = tempRoute[index];
//            tempIndex++;
//        }
//        tempIndex = 0;
//        for (int index = x; index <= y; index++) {
//            tempRoute[index] = routeTemp[tempIndex];
//            tempIndex++;
//        }
//        delete[] routeTemp;
//        //2-Opt As well
//        twoOptLocalPheromoneAddonSearch(tempRoute);
//        new_route_length = KNN::getRouteLength(tempRoute);
//        //DEBUGGING
////        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
////            printf("%d, ",tempRoute[index]);
////        printf("\n");
//    }
//    decreaseLocalSearchPheromone();
//    if (new_route_length < route_length) {
//        for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//            v[index] = tempRoute[index];
//        localSearchPheromone[getArcCode(x,y)] = localSearchPheromone[getArcCode(x,y)] + (int) ((route_length - new_route_length));
//    }
//    //printLocalSearchPheromones();
//    delete[] tempRoute;
//
//}
//
//std::string KNN::getArcCode(int customerA, int customerB) {
//    char *index = new char[4];
//    if (customerA < customerB)
//        sprintf(index, "%d %d", customerA, customerB);
//    else
//        sprintf(index, "%d %d", customerB, customerA);
//    std::string arcCode(index);
//    delete[] index;
//    return arcCode;
//}
