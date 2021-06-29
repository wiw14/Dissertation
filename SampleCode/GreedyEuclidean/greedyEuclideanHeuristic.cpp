#include<stdlib.h>
#include<string>
#include<limits.h>

#include "../LocalSearchAndGenerateTour/LocalSearches.h"
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
    int currentBest = -1;
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
 * Finds the next valid node if there are node nodes near the current node.
 */
int findNextNode(const bool* validNode){
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        if(!validNode[i])
            return i;
    }
    return -1;
}

/*
 * Finds a route between customers based on a greedy approach which chooses the closest neighbour.
 */
void greedyHeuristic(int randomSearchIteration, int twoOptIteration){
    auto * LS = new localSearch(randomSearchIteration,twoOptIteration);

    int prev, next, chargingStation;
    double activeCapacity = 0.0, activeBatteryLevel = 0.0;
    int* nextNode;
    bool* validNode;

    validNode = new bool[NUM_OF_CUSTOMERS+1];
    nextNode = new int[NUM_OF_CUSTOMERS+1];
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        validNode[i] = false;

    int startIndex = rand()%(NUM_OF_CUSTOMERS-1);
    validNode[startIndex] = true;
    nextNode[0] = startIndex;

    //Loop through all the customers finding the closest node.
    for (int index = 1; index <= NUM_OF_CUSTOMERS; index++){
        int temp = findClosestNode(validNode,nextNode[index-1]);
        if (temp == -1)
            temp = findNextNode(validNode);
        nextNode[index] = temp;
        validNode[nextNode[index]] = true;
    }

    //Run local search on the current route.
    LS->randomPheromoneLocalSearchWithTwoOpt(nextNode);
//
    //Evaluate route.
    GenerateTour::getRouteLength(nextNode);
    delete LS;
    delete[] validNode;
    delete[] nextNode;
}
