#include "ACOOnClusters.h"
#include "../Framework/stats.hpp"

int *generateCostTable(int sizeOfAlphabet, std::vector<int *> clusters, int *sizes) {
    auto **costTable = new double *[sizeOfAlphabet];
    for (int i = 0; i < sizeOfAlphabet; ++i) {
        costTable[i] = new double[sizeOfAlphabet];
        for (int j = 0; j < sizeOfAlphabet; ++j) {
            costTable[i][j] = 0;
        }
    }

    for (int i = 0; i < sizeOfAlphabet; ++i) {
        for (int j = 0; j < sizeOfAlphabet; ++j) {
            if (i != j) {
                costTable[i][j] = get_distance(clusters.at(i)[sizes[i] - 1], clusters.at(j)[0]);
            }
        }
    }

    auto min = std::pair<int, int>();
    double minVal = INT_MAX;
    for (int i = 0; i < sizeOfAlphabet; ++i) {
        for (int j = 0; j < sizeOfAlphabet; ++j) {
            if (costTable[i][j] < minVal) {
                minVal = costTable[i][j];
                min.first = i;
                min.second = j;
            }
        }
    }

    int *finalRoute = new int[NUM_OF_CUSTOMERS + 1];
    int count = 0;
    finalRoute[count++] = DEPOT;
    auto addToRoute = [&]() {
        for (int customers = 0; customers < sizes[min.second]; ++customers) {
            finalRoute[count++] = clusters.at(min.second)[customers];
        }
    };
//    addToRoute();
    bool* visited = new bool[sizeOfAlphabet];
    for (int i = 0; i < sizeOfAlphabet; ++i) {
        visited[i] = false;
    }

    for (int customers = 0; customers < sizes[min.first]; ++customers) {
        finalRoute[count++] = clusters.at(min.first)[customers];
    }
    for (int i = 0; i < sizeOfAlphabet-1; ++i) {
        visited[min.first] = true;
        visited[min.second] = true;
        minVal =INT_MAX;
        int tempIndex = min.second;

        for (int j = 0; j < sizeOfAlphabet; ++j) {
            if(!visited[j] && costTable[tempIndex][j] < minVal){
                minVal = costTable[tempIndex][j];
                min.first = tempIndex;
                min.second = j;
            }
        }
        addToRoute();
    }

    for (int i = 0; i < sizeOfAlphabet; ++i) {
        delete[] costTable[i];
    }
    delete[] costTable;
    delete[] visited;
    return finalRoute;
}

//int* placeChargingStations(){
//
//}

int *generateRouteFromClusters(int numClusters, std::vector<int *> clusters, int *sizes){
    int* route = new int[NUM_OF_CUSTOMERS+NUM_OF_CUSTOMERS];
    int routeIndex = 0;
//    route[routeIndex++] = 0;
    for (int clusterCounter = 0; clusterCounter < numClusters; ++clusterCounter) {
        for (int customerCounter = 0; customerCounter < sizes[clusterCounter]; ++customerCounter) {
//            printf("customer : %d\n",clusters.at(clusterCounter)[customerCounter]);
            route[routeIndex++] = clusters.at(clusterCounter)[customerCounter];
        }
//        route[routeIndex++] = 0;
    }
    route[NUM_OF_CUSTOMERS] = 0;
//    for (int i = 0; i <=  NUM_OF_CUSTOMERS; ++i) {
//        printf("=%d, ",route[i]);
//    }printf("\n");
    return route;
}

void ACOOnClusters() {
    int numAnts = 8, iterations = 50, probabilityArraySize = 2, twoOptIteration = 3, randomSearchIteration = 10;
    double pheromoneDecrease = 0.98, Q = 1, alpha = 0.6, beta = 0.6;

//    int numAnts = 4, iterations = 15, probabilityArraySize = 2, twoOptIteration = 3, randomSearchIteration = 3;
//    double pheromoneDecrease = 0.9, Q = 1, alpha = 0.9, beta = 0.9;

    auto cluster = new Cluster();
    auto clusters = std::vector<int *>();
    int *clusterSizes = new int[cluster->numOfClusters];
    for (int clusterIndex = 0; clusterIndex < cluster->numOfClusters; ++clusterIndex) {
        if(cluster->clusters->at(clusterIndex)->sizeOfCluster > 1) {
            auto clusterACO = new ClusterACO(numAnts, pheromoneDecrease, Q, probabilityArraySize, alpha, beta,
                                             twoOptIteration, randomSearchIteration,
                                             cluster->clusters->at(clusterIndex)->customers,
                                             cluster->clusters->at(clusterIndex)->sizeOfCluster);

//        for (int i = 0; i < cluster->clusters->at(clusterIndex)->sizeOfCluster; ++i)
//            printf("%d, ", cluster->clusters->at(clusterIndex)->customers[i]);
//        printf("\n");

            clusterACO->optimize(iterations);
            int *route = clusterACO->returnResults();
            clusterSizes[clusterIndex] = cluster->clusters->at(clusterIndex)->sizeOfCluster;
            twoOptForCluster(route,clusterSizes[clusterIndex],5);
            clusters.push_back(route);

//        for (int i = 0; i < cluster->clusters->at(clusterIndex)->sizeOfCluster; ++i)
//            printf("%d, ", route[i]);
//        printf("\n");
//        printf("\n");

            delete clusterACO;
        }
        else{
            clusterSizes[clusterIndex] = cluster->clusters->at(clusterIndex)->sizeOfCluster;
            clusters.push_back(cluster->clusters->at(clusterIndex)->customers);
        }
    }
//    printf("Cluster Gen Complete\n");

//    int* r = generateCostTable(cluster->numOfClusters, clusters, clusterSizes);
    int* r = generateRouteFromClusters(cluster->numOfClusters,clusters,clusterSizes);
//    for (int i = 0; i <=  NUM_OF_CUSTOMERS; ++i) {
//        printf("%d, ",r[i]);
//    }printf("\n");

int improve = 0;
    auto* LS = new localSearch(3,3);
        GenerateTour::getRouteLength(r);
//    for (int i = 0; i < 500; ++i) {
////        int* temp = new int[NUM_OF_CUSTOMERS+1];
////        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
////            temp[j] = r[j];
////        }
//        double currentBest = best_sol->tour_length;
//        LS->randomPheromoneLocalSearchWithTwoOpt(r);
//        addRunDataToFile(i,best_sol->tour_length);
////        printf("%f\n",best_sol->tour_length);
//        if(best_sol->tour_length < currentBest){
//            improve = 0;
//        }
//        improve++;
//        if(improve > randomSearchIteration){
//            break;
//        }
////        bool change = false;
////        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
////            if(r[j] != temp[j])
////                change = true;
////        }
////        delete[]temp;
////        if(change)
////            break;
//    }
//    GenerateTour::getRouteLength(r);
//    printf("Final: %f\n",best_sol->tour_length);
    delete LS;
    delete cluster;
}

void twoOptForCluster(int* bestRoute, int clusterSize, int twoOptIterations) {
    int improve = 0;
    int *tempRoute = new int[clusterSize];
    //Checks whether there has been an improvement within x number of iterations.
    while (improve < twoOptIterations) {
        for (int index = 0; index < clusterSize; index++)
            tempRoute[index] = bestRoute[index];
        double route_length = getClusterRouteLength(bestRoute,clusterSize);
        for (int i = 0; i < clusterSize - 1; ++i) {
            for (int j = i + 1; j < clusterSize; ++j) {
                //Swaps the route between index i and j.
                twoOptSwapForClusters(i, j, tempRoute, bestRoute, clusterSize);
                double new_route_length = getClusterRouteLength(tempRoute,clusterSize);
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

void twoOptSwapForClusters(int i, int j, int *route, const int *currRoute,int routeSize) {
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

double getClusterRouteLength(int* route, int clusterSize){
    double totalDist = 0;
    for (int i = 0; i < clusterSize-1; ++i)
        totalDist += get_distance(route[i],route[i+1]);
    return totalDist;
}