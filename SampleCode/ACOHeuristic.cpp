//
// Created by wmw13 on 05/02/2021.
//

#include<cmath>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<math.h>
#include<fstream>
#include<limits.h>

#include "heuristic.hpp"

#include "ACOHeuristic.h"
#include "ACOv1.h"
#include "ACO.h"

void generateACOTour(const int *nextNode) {

    /*
    * Re-Initialise best_sol
    */
    best_sol->steps = 0;
    best_sol->tour_length = INT_MAX;

    /*
     * Sets the first item in the tour to DEPOT because all routes start at the depot.
     * Increment steps to 1 due to first step was DEPOT.
     */
    best_sol->tour[0] = DEPOT;
    best_sol->steps++;

    int prev, next, chargingStation;
    double activeCapacity = 0.0, activeBatteryLevel = 0.0;
    int i = 1;
    while (i <= NUM_OF_CUSTOMERS) {
        prev = best_sol->tour[best_sol->steps - 1];
        next = nextNode[i];
        if ((activeCapacity + get_customer_demand(next)) <= MAX_CAPACITY &&
            activeBatteryLevel + get_energy_consumption(prev, next) <= BATTERY_CAPACITY) {
            activeCapacity += get_customer_demand(next);
            activeBatteryLevel += get_energy_consumption(prev, next);
            best_sol->tour[best_sol->steps] = next;
            best_sol->steps++;
            i++;
        } else if ((activeCapacity + get_customer_demand(next)) > MAX_CAPACITY) {
            activeCapacity = 0.0;
            activeBatteryLevel = 0.0;
            best_sol->tour[best_sol->steps] = DEPOT;
            best_sol->steps++;
        } else if (activeBatteryLevel + get_energy_consumption(prev, next) > BATTERY_CAPACITY) {
//            chargingStation = rand() % (ACTUAL_PROBLEM_SIZE - NUM_OF_CUSTOMERS - 1) + NUM_OF_CUSTOMERS + 1;
            chargingStation = NUM_OF_CUSTOMERS+NUM_OF_STATIONS;
            for (int index = NUM_OF_CUSTOMERS+1; index <= (NUM_OF_CUSTOMERS+NUM_OF_STATIONS); index++){
                if (get_distance(next,index) < get_distance(next,chargingStation) && is_charging_station(index))
                    chargingStation = index;
            }
            if (is_charging_station(chargingStation)) {
                activeBatteryLevel = 0.0;
                best_sol->tour[best_sol->steps] = chargingStation;
                best_sol->steps++;
            }
        } else {
            activeCapacity = 0.0;
            activeBatteryLevel = 0.0;
            best_sol->tour[best_sol->steps] = DEPOT;
            best_sol->steps++;
        }
    }

    //close EVRP tour to return back to the depot
    if (best_sol->tour[best_sol->steps - 1] != DEPOT) {
        best_sol->tour[best_sol->steps] = DEPOT;
        best_sol->steps++;
    }

    best_sol->tour_length = fitness_evaluation(best_sol->tour, best_sol->steps);
}

//void ACOHeuristic(){
//    int numAnts = 3,Taumax = 2, Iterations = 5000;  //Original Values: nA=4, T=2, I=5 //Best Values: nA=3, T=2, I=5
//    double Alpha = 0.28, Beta = 0.8, Q = 80, RO = 0.8; //Original Values: A=0.5, B=0.8, Q=80, RO=0.2 //Best Values: A=0.28, B=0.8, Q=80, RO=0.8
//
//    ACOv1 *ants = new ACOv1 (numAnts, NUM_OF_CUSTOMERS + 1,
//                             Alpha, Beta, Q, RO, Taumax,
//                             DEPOT);
//
//    ants -> init();
//
//    for (int startCustomer=0;startCustomer<=NUM_OF_CUSTOMERS; startCustomer++){
//        for(int endCustomer=startCustomer+1;endCustomer<=NUM_OF_CUSTOMERS;endCustomer++){
//            ants->connectCustomers(startCustomer, endCustomer);
//        }
//
//        node currentNode = getNodeInfo(startCustomer);
//        ants->setCustomerLocation(startCustomer, currentNode.x, currentNode.y);
//    }
//
//    ants -> optimize (Iterations);
//    generateACOTour(ants->returnResults());
//
//}
void ACOHeuristic(){
    int numAnts=3, iterations = 1000, probabilityArraySize = 2;
    double pheromoneDecrease = 0.8, Q = 80,alpha = 0.8, beta=0.8;
    ACO* a = new ACO(numAnts,pheromoneDecrease,Q,probabilityArraySize,alpha,beta);
    //printf("ACO Initialised\n"); //DEBUGGING
    a->optimize(iterations);
    //printf("ACO Optimized\n"); //DEBUGGING
    int * route = a->returnResults();
    //DEBUGGING
//    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++)
//        printf("%d, ",route[index]);
//    printf("\n");
    generateACOTour(route);
}