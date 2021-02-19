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

#include "LocalSearches.h"
#include "greedyEuclideanHeuristic.h"

/*
 * ================================================================================ *
 * GREEDY HEURISTIC, USING EUCLIDEAN DISTANCE
 * ================================================================================ *
 */

/*
 *
 */
int findClosestNode(const bool* validNode, int anchor){
    double HEURISTIC_VALUE = 2.0;
    double shortDist = INT_MAX, dist;
    int currentBest = 0;
    for(int index = 0; index <=NUM_OF_CUSTOMERS;index++){
        if(!validNode[index]){
            dist = get_distance(anchor,index);
            if(shortDist > dist){
                currentBest = index;
                shortDist = dist;
                if(shortDist <= HEURISTIC_VALUE)
                    return currentBest;
            }
        }
    }
    return currentBest;
}

/*
 *
 */
void greedyHeuristic(){
    auto * LS = new localSearch(3,3);
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
    int* nextNode;
    bool* validNode;

    validNode = new bool[NUM_OF_CUSTOMERS+1];
    nextNode = new int[NUM_OF_CUSTOMERS+1];

    validNode[DEPOT] = true;

    nextNode[0] = findClosestNode(validNode,DEPOT);

    validNode[nextNode[0]] = true;

    for (int index = 1; index <= NUM_OF_CUSTOMERS; index++){
        nextNode[index] = findClosestNode(validNode,nextNode[index-1]);
        validNode[nextNode[index]] = true;
    }
    LS->randomPheromoneLocalSearchWithTwoOpt(nextNode);
    int i = 0;
    while(i < NUM_OF_CUSTOMERS) {
        prev = best_sol->tour[best_sol->steps-1];
        next = nextNode[i];
        if((activeCapacity + get_customer_demand(next)) <= MAX_CAPACITY && activeBatteryLevel+get_energy_consumption(prev,next) <= BATTERY_CAPACITY){
            activeCapacity  += get_customer_demand(next);
            activeBatteryLevel += get_energy_consumption(prev,next);
            best_sol->tour[best_sol->steps] = next;
            best_sol->steps++;
            i++;
        } else if ((activeCapacity + get_customer_demand(next)) > MAX_CAPACITY){
            activeCapacity = 0.0;
            activeBatteryLevel = 0.0;
            best_sol->tour[best_sol->steps] = DEPOT;
            best_sol->steps++;
        } else if (activeBatteryLevel+get_energy_consumption(prev,next) > BATTERY_CAPACITY){
            chargingStation = rand() % (ACTUAL_PROBLEM_SIZE-NUM_OF_CUSTOMERS-1)+NUM_OF_CUSTOMERS+1;
            if(is_charging_station(chargingStation)){
                activeBatteryLevel = 0.0;
                best_sol->tour[best_sol->steps] =  chargingStation;
                best_sol->steps++;
            }
        } else {
            activeCapacity = 0.0;
            activeBatteryLevel = 0.0;
            best_sol->tour[best_sol->steps] =  DEPOT;
            best_sol->steps++;
        }
    }

    //close EVRP tour to return back to the depot
    if(best_sol->tour[best_sol->steps-1]!=DEPOT){
        best_sol->tour[best_sol->steps] = DEPOT;
        best_sol->steps++;
    }

    best_sol->tour_length = fitness_evaluation(best_sol->tour, best_sol->steps);

}
