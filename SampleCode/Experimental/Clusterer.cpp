//
// Created by wmw13 on 05/03/2021.
//

#include <climits>
#include "Clusterer.h"

/*
 * Initialises static variables.
 */
int Clusterer::k;
struct Clusterer::Node** Clusterer::clusters;
int Clusterer::numOfClusters;
int Clusterer::twoOptIterations = 10;

/*
 * Calculates the length of the route by summing the cluster distances.
 */
double Clusterer::getRouteLength(int* clusterRoute) {
    double length = 0.0;
    for (int i = 1; i < numOfClusters; ++i) {
        length+=getClosestDistance(clusterRoute[i-1],clusterRoute[i]);
        length+=clusterRoute[i-1];
    }
    return length;
}

/*
 * Frees all the memory used for the clusters.
 */
void Clusterer::freeClusters() {
    for (int i = 0; i < numOfClusters; ++i) {
        delete[] clusters[i]->customers;
        delete clusters[i];
    }
    delete[] clusters;
}

/*
 * Determines which customers should be in the cluster based upon the initial customer.
 */
int * Clusterer::getKNN(int customer, int KNN,const bool* visited) {
    int *neighbour = new int[KNN+1];
    auto* minDist = new double[KNN];
    for (int i = 0; i < KNN; ++i) {
        neighbour[i] = -1;
        minDist[i] = INT_MAX;
    }
    neighbour[KNN] = -1;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        if(customer != i && !visited[i]){
            double currentDist = get_distance(customer,i);
            if(currentDist < minDist[KNN-1]){
                for (int j = 0; j < KNN; ++j) {
                    if(currentDist < minDist[j]){
                        for (int l = KNN-1; l > j; --l) {
                            minDist[l] = minDist[l-1];
                            neighbour[l] = neighbour[l-1];
                        }
                        minDist[j] = currentDist;
                        neighbour[j] = i;
                        break;
                    }
                }
            }
        }
    }
    delete[] minDist;
    return neighbour;
}

/*
 * Generates all the clusters based on a k value.
 */
void Clusterer::createClusters(int K){
    k = K;
    clusters = new struct Node*[((NUM_OF_CUSTOMERS+1)/K)+1];
    numOfClusters = 0;

    bool* visited = new bool[NUM_OF_CUSTOMERS+1];
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
        visited[i] = false;

    for (int customer = 0; customer <=NUM_OF_CUSTOMERS; ++customer) {
        if(!visited[customer]) {
            clusters[numOfClusters] = new struct Node;
            clusters[numOfClusters]->customers = getKNN(customer,k,visited);
            clusters[numOfClusters]->sizeOfCluster = 0;
            clusters[numOfClusters]->distance = 0.0;
            for (int i = 0; i < k; ++i) {
                if(clusters[numOfClusters]->customers[i] == -1)
                    break;
                clusters[numOfClusters]->sizeOfCluster++;
            }
            clusters[numOfClusters]->customers[clusters[numOfClusters]->sizeOfCluster] = customer;
            clusters[numOfClusters]->sizeOfCluster++;
            for (int i = 0; i < clusters[numOfClusters]->sizeOfCluster; ++i) {
                if(i != 0)
                    clusters[numOfClusters]->distance += get_distance(clusters[numOfClusters]->customers[i-1],clusters[numOfClusters]->customers[i]);
                visited[clusters[numOfClusters]->customers[i]] = true;
            }
            //optimiseCluster(clusters[numOfClusters]);
            numOfClusters++;
        }
    }
    delete[] visited;
}

/*
 * Displays all the cluster information for each cluster..
 */
void Clusterer::displayClusters(){
    for (int i = 0; i < numOfClusters; ++i) {
        printf("Cluster %d: \n",i+1);
        printf("Customers: ");
        for (int j = 0; j < clusters[i]->sizeOfCluster; ++j)
            printf("%d, ",clusters[i]->customers[j]);
        printf("\n");
        printf("Distance: %f\n",clusters[i]->distance);
        printf("\n");
    }
}

/*
 * Gets the distance between the closest nodes in the clusters.
 */
double Clusterer::getClosestDistance(int posA, int posB){
    double minDist = INT_MAX;
    for (int i = 0; i < clusters[posA]->sizeOfCluster; ++i) {
        for (int j = 0; j < clusters[posB]->sizeOfCluster; ++j) {
            double currentDist = get_distance(clusters[posA]->customers[i],clusters[posB]->customers[j]);
            if(currentDist < minDist)
                minDist = currentDist;
        }

    }
    return minDist;
}

/*
 * Generates a customer route by appending the clusters together using the order determined in the cluster route.
 */
int* Clusterer::getRouteFromClusters(int* clusterRoute){
    int* route = new int[NUM_OF_CUSTOMERS+1], counter = 0;
    for (int i = 0; i < numOfClusters; ++i) {
        for (int j = 0; j < clusters[clusterRoute[i]]->sizeOfCluster; ++j) {
            route[counter++] = clusters[clusterRoute[i]]->customers[j];
        }
    }
    //delete[] clusterRoute;
    return route;
}

/*
 * Generates teh distance within the cluster.
 */
double Clusterer::calculateClusterDistance(int* customers,int size){
    double length = 0.0;
    for (int i = 0; i < size-1; ++i)
        length+= get_distance(customers[i],customers[i+1]);
    return length;
}

/*
 * =================================================================================================================== *
 * CLUSTER OPTIMISATION.
 * =================================================================================================================== *
 */

/*
 * Optimises the order of the customers within the cluster.
 */
void optimiseCluster(struct Node* node){

}

/*
 * Swaps the route between the inputted points used in 2-opt local search.
 */
//void Clusterer::twoOptSwap(int i, int j, int *route, const int *currRoute) {
//    for (int l = 0; l < i; ++l)
//        route[l] = currRoute[l];
//    int index = i;
//    for (int l = j; l >= i; --l) {
//        route[index] = currRoute[l];
//        index++;
//    }
//    for (int l = j + 1; l <= NUM_OF_CUSTOMERS; ++l) {
//        route[l] = currRoute[l];
//    }
//}

/*
 * Optimises the order of the customers within the cluster, using two-opt local search.
 */
//void Clusterer::optimiseCluster(struct Node* node){
////    printf("Start Optimise\n");
//    int* bestRoute = node->customers;
//    int improve = 0;
//    int *tempRoute = new int[node->sizeOfCluster];
//    //Checks whether there has been an improvement within x number of iterations.
//    while (improve < twoOptIterations) {
//        for (int index = 0; index < node->sizeOfCluster; index++)
//            tempRoute[index] = bestRoute[index];
//        double route_length = Clusterer::calculateClusterDistance(bestRoute,node->sizeOfCluster);
//        for (int i = 0; i < node->sizeOfCluster-1; ++i) {
//            for (int j = i + 1; j < node->sizeOfCluster; ++j) {
//                //Swaps the route between index i and j.
//                twoOptSwap(i, j, tempRoute, bestRoute);
//                double new_route_length = Clusterer::calculateClusterDistance(tempRoute, node->sizeOfCluster);
//                if (new_route_length < route_length) {
//                    improve = 0;
//                    for (int index = 0; index < node->sizeOfCluster; index++)
//                        bestRoute[index] = tempRoute[index];
//                    route_length = new_route_length;
//                }
//            }
//        }
//        improve++;
//    }
//    node->distance = Clusterer::calculateClusterDistance(bestRoute,node->sizeOfCluster);
////    printf("Finish Optimise\n");
////
////    for (int i = 0; i < node->sizeOfCluster; ++i) {
////        printf("%d, ",node->customers[i]);
////    }printf("\n");
//
//    //De-allocates the memory used above.
//    //delete[] tempRoute;
//}
