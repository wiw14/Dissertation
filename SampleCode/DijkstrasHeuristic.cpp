//
// Created by wmw13 on 06/02/2021.
//

#include<cmath>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<fstream>
#include<limits.h>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

#include "EVRP.hpp"
#include "heuristic.hpp"

#include "DijkstrasHeuristic.h"

#define KNN 1

bool *visited;

/*
 * adjList is nodes connected to current node
 * cluster design?
 * closest 5 nodes or so
 */

int **findAdjacentNodes(int center) {
    int **closestNodes;
    double dist;
    closestNodes = new int *[KNN];
    for (int index = 0; index < KNN; ++index) {
        closestNodes[index] = new int[2];
        closestNodes[index][0] = -1;
        closestNodes[index][1] = 9999;
    }
    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++) {
        if (index != center && !visited[index]) {
            dist = get_distance(center, index);
            for (int subIndex = 0; subIndex < KNN; subIndex++) {
                if (closestNodes[subIndex][1] > dist) {
                    closestNodes[subIndex][0] = index;
                    closestNodes[subIndex][1] = (int) dist;
                    break;
                }
            }
        }
    }
    return closestNodes;
}

void generateTour(const int *nextNode) {
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
    int i = 0;
    while (i < NUM_OF_CUSTOMERS) {
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
            chargingStation = rand() % (ACTUAL_PROBLEM_SIZE - NUM_OF_CUSTOMERS - 1) + NUM_OF_CUSTOMERS + 1;
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

int* Dijkstra(){
    int start = DEPOT;
    int *shortestPath;
    shortestPath = new int[NUM_OF_CUSTOMERS + 1];
    shortestPath[start] = 0;

    int nodeWeight, nodeIndex;

    visited = new bool[NUM_OF_CUSTOMERS + 1];
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        visited[i] = false;
    }

    for (int index = 1; index <= NUM_OF_CUSTOMERS; index++) {
        shortestPath[index] = 100000; //Set shortestPath to infinity
    }

    int *sourcePair = new int[2];
    int *currentNode = new int[2];

    sourcePair[0] = start;
    sourcePair[1] = 0;

    priority_queue<int *> nodeQueue;
    nodeQueue.push(sourcePair);

    while (!nodeQueue.empty()) {
        currentNode = nodeQueue.top();

        nodeIndex = currentNode[0];
        nodeWeight = currentNode[1];

        nodeQueue.pop();



        if (visited[currentNode[0]])
            continue;

        visited[currentNode[0]] = true;

        int **closestNodes = findAdjacentNodes(currentNode[0]);
        for (int index = 0; index < KNN; index++) {
            if (closestNodes[index][0] != -1) {
                if ((nodeWeight + closestNodes[index][1]) < shortestPath[closestNodes[index][0]]) {
                    shortestPath[closestNodes[index][0]] = nodeWeight + closestNodes[index][1];
                    int *tempNode = new int[2];
                    tempNode[0] = closestNodes[index][0];
                    tempNode[1] = shortestPath[closestNodes[index][0]];
                    nodeQueue.push(tempNode);
                }
            }
        }

        delete[] closestNodes;
    }
    bool *checked;
    int *nextNode;
    checked = new bool[NUM_OF_CUSTOMERS + 1];
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        checked[i] = false;
    }
    nextNode = new int[NUM_OF_CUSTOMERS];
    int *currentBest = new int[2];
    currentBest[0] = -1;
    currentBest[1] = 9999;

    /*
     * Debugging
     */
//    for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
//        printf("path->%d ", shortestPath[i]);
//    }
//    printf("\n");

    for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
        for (int j = 1; j <= NUM_OF_CUSTOMERS; j++) {
            if (!checked[j] && shortestPath[j] < currentBest[1]) {
                currentBest[1] = shortestPath[j];
                currentBest[0] = j;
            }
        }
        nextNode[i] = currentBest[0];
        checked[currentBest[0]] = true;
        currentBest = new int[2];
        currentBest[0] = -1;
        currentBest[1] = 9999;

    }

    /*
     * Debugging
     */
//    for (int i = 0; i < NUM_OF_CUSTOMERS; ++i) {
//        printf("node->%d ", nextNode[i]);
//    }
//    printf("\n");
return nextNode;
}

void DijkstrasHeuristic() {
    generateTour(Dijkstra());
}