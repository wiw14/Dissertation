#include <queue>
#include <functional>
#include <climits>

using namespace std;

#include "../LocalSearchAndGenerateTour/LocalSearches.h"

#include "DijkstrasHeuristic.h"

#define KNN 5

/*
 * ================================================================================ *
 * DIJKSTRA HEURISTIC
 * ================================================================================ *
 */

//Stores whether the node is visited.
bool *visited;

/*
 * Finds numOfClusters nearest neighbours to the inputted customer.
 */
int **findAdjacentNodes(int center) {
    int **closestNodes;
    double dist;

    //Initiates closestNode array.
    closestNodes = new int *[KNN];
    for (int index = 0; index < KNN; ++index) {
        closestNodes[index] = new int[2];
        closestNodes[index][0] = -1;
        closestNodes[index][1] = INT_MAX;
    }

    //Loop through all customers and find numOfClusters closest neighbours.
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

/*
 * Dijkstra algorithm used to find route between customers.
 */
int *Dijkstra() {
    int start = DEPOT;
    int *shortestPath;
    shortestPath = new int[NUM_OF_CUSTOMERS + 1];
    shortestPath[start] = 0;

    int nodeWeight, nodeIndex;

    visited = new bool[NUM_OF_CUSTOMERS + 1];
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        visited[i] = false;
    }

    //Sets the shortestPaths to infinity.
    for (int index = 1; index <= NUM_OF_CUSTOMERS; index++) {
        shortestPath[index] = INT_MAX;
    }

    int *sourcePair = new int[2];
    int *currentNode = new int[2];

    sourcePair[0] = start;
    sourcePair[1] = 0;

    priority_queue<int *> nodeQueue;
    nodeQueue.push(sourcePair);

    //Loops through all the customers.
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
        for (int i = 0; i < KNN; ++i)
            delete[] closestNodes[i];
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


    nextNode[0] = 0;
    for (int i = 1; i <= NUM_OF_CUSTOMERS; ++i) {
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

    //Frees memory used for the Dijkstra Algorithm.
    delete[] currentBest;
    delete[] checked;
    delete[] shortestPath;
    delete[] sourcePair;
    delete[] currentNode;
    return nextNode;
}

/*
 * Creates local search object, finds tour between customers, runs local search, then evaluates the tour.
 */
void DijkstrasHeuristic(int randomSearchIteration,int twoOptIteration) {
    auto *LS = new localSearch(randomSearchIteration, twoOptIteration);
    int *tour = Dijkstra();
    LS->randomPheromoneLocalSearchWithTwoOpt(tour);
    double val = GenerateTour::getRouteLength(tour);
    delete LS;
}