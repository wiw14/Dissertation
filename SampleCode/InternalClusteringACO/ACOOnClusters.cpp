#include "ACOOnClusters.h"
#include "../Framework/stats.hpp"

/*
 * Generates a route from the clusters.
 */
int *generateRouteFromClusters(int numClusters, std::vector<int *> clusters, int *sizes) {
    int *route = new int[NUM_OF_CUSTOMERS + NUM_OF_CUSTOMERS];
    int routeIndex = 0;
    for (int clusterCounter = 0; clusterCounter < numClusters; ++clusterCounter) {
        for (int customerCounter = 0; customerCounter < sizes[clusterCounter]; ++customerCounter) {
            route[routeIndex++] = clusters.at(clusterCounter)[customerCounter];
        }
    }
    route[NUM_OF_CUSTOMERS] = 0;
    return route;
}

/*
 * Internal Clustered Ant Colony Optimisation, applies the ACO to each cluster to generate an optimal route.
 */
void ACOOnClusters(int nAnts, int iter, int probSize, double pheroDec, double q, double al, double be, int rsi, int toi) {

    auto cluster = new Cluster();
    auto clusters = std::vector<int *>();
    int *clusterSizes = new int[cluster->numOfClusters];
    for (int clusterIndex = 0; clusterIndex < cluster->numOfClusters; ++clusterIndex) {
        printf("Cluster %d\n",clusterIndex);
        if (cluster->clusters->at(clusterIndex)->sizeOfCluster > 1) {
            auto clusterACO = new ClusterACO(nAnts, pheroDec, q, probSize, al, be,
                                             toi, rsi,
                                             cluster->clusters->at(clusterIndex)->customers,
                                             cluster->clusters->at(clusterIndex)->sizeOfCluster);

            clusterACO->optimize(iter);
            int *route = clusterACO->returnResults();
            clusterSizes[clusterIndex] = cluster->clusters->at(clusterIndex)->sizeOfCluster;
            twoOptForCluster(route, clusterSizes[clusterIndex], toi);
            clusters.push_back(route);

            delete clusterACO;
        } else {
            clusterSizes[clusterIndex] = cluster->clusters->at(clusterIndex)->sizeOfCluster;
            clusters.push_back(cluster->clusters->at(clusterIndex)->customers);
        }
    }

    int *r = generateRouteFromClusters(cluster->numOfClusters, clusters, clusterSizes);

    //Route improvement by iteratively running local search on the generated route.
    int improve = 0;
    auto *LS = new localSearch(rsi, toi);
    GenerateTour::getRouteLength(r);
    for (int i = 0; i < 50; ++i) {
        double currentBest = best_sol->tour_length;
        //Local Search.
        LS->randomPheromoneLocalSearchWithTwoOpt(r);
        addRunDataToFile(i, best_sol->tour_length);
        if (best_sol->tour_length < currentBest) {
            improve = 0;
        }
        improve++;
        //If no improvement after x iterations exit for loop.
        if (improve > rsi) {
            break;
        }
    }
    //Update best route using generate tour.
    GenerateTour::getRouteLength(r);
    delete LS;
    delete cluster;
}

/*
 * 2-Opt local search for the reduced variable size of the clusters.
 */
void twoOptForCluster(int *bestRoute, int clusterSize, int twoOptIterations) {
    int improve = 0;
    int *tempRoute = new int[clusterSize];
    //Checks whether there has been an improvement within x number of iterations.
    while (improve < twoOptIterations) {
        for (int index = 0; index < clusterSize; index++)
            tempRoute[index] = bestRoute[index];
        double route_length = getClusterRouteLength(bestRoute, clusterSize);
        for (int i = 0; i < clusterSize - 1; ++i) {
            for (int j = i + 1; j < clusterSize; ++j) {
                //Swaps the route between index i and j.
                twoOptSwapForClusters(i, j, tempRoute, bestRoute, clusterSize);
                double new_route_length = getClusterRouteLength(tempRoute, clusterSize);
                if (new_route_length < route_length) {
                    improve = 0;
                    for (int index = 0; index < clusterSize; index++)
                        bestRoute[index] = tempRoute[index];
                    route_length = new_route_length;
                }
            }
        }
        improve++;
    }

    //De-allocates the memory used above.
    delete[] tempRoute;
}

/*
 * The swap function for the clustered 2-Opt local search.
 */
void twoOptSwapForClusters(int i, int j, int *route, const int *currRoute, int routeSize) {
    for (int k = 0; k < i; ++k)
        route[k] = currRoute[k];
    int index = i;
    for (int k = j; k >= i; --k) {
        route[index] = currRoute[k];
        index++;
    }
    for (int k = j + 1; k < routeSize; ++k) {
        route[k] = currRoute[k];
    }
}

/*
 * Gets the route length of a cluster by iterating through all the customers in a cluster.
 */
double getClusterRouteLength(int *route, int clusterSize) {
    double totalDist = 0;
    for (int i = 0; i < clusterSize - 1; ++i)
        totalDist += get_distance(route[i], route[i + 1]);
    return totalDist;
}