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
 * Finds the closest neighbour which hasn't been visited.
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
 * Finds a route between customers based on a greedy approach which chooses the closest neighbour.
 */
void greedyHeuristic(){
    auto * LS = new localSearch(3,10);

    int prev, next, chargingStation;
    double activeCapacity = 0.0, activeBatteryLevel = 0.0;
    int* nextNode;
    bool* validNode;

    validNode = new bool[NUM_OF_CUSTOMERS+1];
    nextNode = new int[NUM_OF_CUSTOMERS+1];

    validNode[DEPOT] = true;

    //Finds the closest node to the depot.
    nextNode[0] = findClosestNode(validNode,DEPOT);

    validNode[nextNode[0]] = true;

    //Loop through all the customers finding the closest node.
    for (int index = 1; index <= NUM_OF_CUSTOMERS; index++){
        nextNode[index] = findClosestNode(validNode,nextNode[index-1]);
        validNode[nextNode[index]] = true;
    }

    //Run local search on the current route.
    LS->randomPheromoneLocalSearchWithTwoOpt(nextNode);
//
    //Evaluate route.
    LS->getRouteLength(nextNode);
    delete LS;
    delete[] validNode;
    delete[] nextNode;
}
