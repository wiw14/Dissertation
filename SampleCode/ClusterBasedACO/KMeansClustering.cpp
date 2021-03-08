//
// Created by wmw13 on 08/03/2021.
//

#include "KMeansClustering.h"

/*
 * NODE FUNCTIONS.
 */
std::string KMeansClustering::Node::displayNode() {
    auto getCustomerString = [&]() {
        std::string output;
        for (int i = 0; i < sizeOfCluster; ++i)
            output += std::to_string(customers[i]) + ", ";
        return output;
    };

    std::string output =
            "Cluster:\nSize of Cluster: " + std::to_string(sizeOfCluster) + "\nDistance: " + std::to_string(distance) +
            "\nCustomers: " + getCustomerString() + "\n";
    return output;
}

void KMeansClustering::Node::getTotalDistance() {
    double length = 0.0;

    for (int customerIndex = 0; customerIndex < sizeOfCluster - 1; ++customerIndex)
        length += customers[customerIndex];

    distance = length;
}

/*
 * BODY OF CLASS.
 */

int KMeansClustering::numOfClusters;
int *KMeansClustering::centroids;
bool *KMeansClustering::visited;
struct KMeansClustering::Node **KMeansClustering::clusters;
int KMeansClustering::twoOptIterations = 3;

void KMeansClustering::initialCentroids() {
    for (int centroidIndex = 0; centroidIndex < numOfClusters; ++centroidIndex) {
        //Loops until it finds a viable centroid.
        do {
            centroids[centroidIndex] = rand() % NUM_OF_CUSTOMERS;

        } while (visited[centroids[centroidIndex]]);
        //Adds the centroid to the visited array.
        visited[centroids[centroidIndex]] = true;
    }
}

void KMeansClustering::createClusters(int k) {
//    printf("Start Clustering...\n");
    numOfClusters = (NUM_OF_CUSTOMERS / k);
    clusters = new struct Node *[numOfClusters];
    for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex) {
        clusters[clusterIndex] = new struct Node;
        clusters[clusterIndex]->customers = new int[NUM_OF_CUSTOMERS];
        clusters[clusterIndex]->sizeOfCluster = 0;
        clusters[clusterIndex]->distance = 0;
        for (int customerIndex = 0; customerIndex < NUM_OF_CUSTOMERS; ++customerIndex)
            clusters[clusterIndex]->customers[customerIndex] = -1;
    }
    centroids = new int[numOfClusters];
    visited = new bool[NUM_OF_CUSTOMERS + 1];
    for (int customerIndex = 0; customerIndex <= NUM_OF_CUSTOMERS; ++customerIndex)
        visited[customerIndex] = false;

//    printf("Initialise Centroids...\n");
    initialCentroids();
//    displayCentroids();
//    printf("Initialise Cluster...\n");
    clusterAroundCentroids();
//    for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
//        optimiseClusters(clusters[clusterIndex]);
//    printf("Finished Clustering\n");
}

//void KMeansClustering::twoOptSwap(int i, int j, int *route, const int *currRoute) {
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
//
//double KMeansClustering::calculateClusterDistance(int* customers,int size){
//    double length = 0.0;
//    for (int i = 0; i < size-1; ++i)
//        length+= get_distance(customers[i],customers[i+1]);
//    return length;
//}
//
//void KMeansClustering::optimiseClusters(struct Node* node){
//    printf("Start Optimise\n");
//    int* bestRoute = node->customers;
//    int improve = 0;
//    int *tempRoute = new int[node->sizeOfCluster];
//    //Checks whether there has been an improvement within x number of iterations.
//    while (improve < twoOptIterations) {
//        for (int index = 0; index < node->sizeOfCluster; index++)
//            tempRoute[index] = bestRoute[index];
//        double route_length = KMeansClustering::calculateClusterDistance(bestRoute,node->sizeOfCluster);
//        for (int i = 0; i < node->sizeOfCluster-1; ++i) {
//            for (int j = i + 1; j < node->sizeOfCluster; ++j) {
//                //Swaps the route between index i and j.
//                twoOptSwap(i, j, tempRoute, bestRoute);
//                double new_route_length = calculateClusterDistance(tempRoute, node->sizeOfCluster);
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
//    node->distance = calculateClusterDistance(bestRoute,node->sizeOfCluster);
//    printf("Finish Optimise\n");
//
//    for (int i = 0; i < node->sizeOfCluster; ++i) {
//        printf("%d, ",node->customers[i]);
//    }printf("\n");
//
//    //De-allocates the memory used above.
//    //delete[] tempRoute;
//}

void KMeansClustering::clusterAroundCentroids() {
    /*
     * STEP 1:
     * Add centroids to clusters.
     */
    for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
        clusters[clusterIndex]->customers[clusters[clusterIndex]->sizeOfCluster++] = centroids[clusterIndex];

    /*
     * STEP 2:
     * Generate Clusters.
     */
    for (int customerIndex = 0; customerIndex <= NUM_OF_CUSTOMERS; ++customerIndex) {
        if(!visited[customerIndex]){
            int closestCluster = -1;
            double closetDistance = INT_MAX;
            for (int centroidIndex = 0; centroidIndex < numOfClusters; ++centroidIndex) {
                double currDist = get_distance(centroids[centroidIndex],customerIndex);
                if(currDist < closetDistance){
                    closetDistance = currDist;
                    closestCluster = centroidIndex;
                }
            }
            clusters[closestCluster]->customers[clusters[closestCluster]->sizeOfCluster++] = customerIndex;
        }
    }

    for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
        clusters[clusterIndex]->getTotalDistance();

//    for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
//        printf("%s\n", clusters[clusterIndex]->displayNode().c_str());
}

double KMeansClustering::getRouteLength(int *clusterRoute) {
    double length = 0.0;
    for (int i = 0; i < numOfClusters; ++i)
        length += clusters[clusterRoute[i]]->distance;
    return length;
}

double KMeansClustering::getClosestDistance(int posA, int posB) {
    double minDist = INT_MAX;
//    printf("Num Clusters : %d, posA: %d, posB: %d\n",numOfClusters,posA,posB);
//    for (int i = 0; i < clusters[posA]->sizeOfCluster; ++i)
//        printf("%d, ",clusters[posA]->customers[i]);
//    for (int i = 0; i < clusters[posB]->sizeOfCluster; ++i)
//        printf("%d, ",clusters[posB]->customers[i]);
    for (int i = 0; i < clusters[posA]->sizeOfCluster; ++i) {
        for (int j = 0; j < clusters[posB]->sizeOfCluster; ++j) {
            double currentDist = get_distance(clusters[posA]->customers[i], clusters[posB]->customers[j]);
            if (currentDist < minDist)
                minDist = currentDist;
        }

    }
//    printf("END DIST %f\n",minDist);
    return minDist;
}

int *KMeansClustering::getRouteFromClusters(const int *clusterRoute) {
    int *route = new int[NUM_OF_CUSTOMERS + 1], counter = 0;
    for (int i = 0; i < numOfClusters; ++i) {
        int *tempClusterRoute = clusters[clusterRoute[i]]->customers;
        for (int j = 0; j < clusters[clusterRoute[i]]->sizeOfCluster; ++j) {
            route[counter++] = tempClusterRoute[j];
        }
    }
    //delete[] clusterRoute;
    return route;
}

//DEBUGGING
void KMeansClustering::displayCentroids() {
    printf("numOfClusters=%d : ", numOfClusters);
    for (int centroidIndex = 0; centroidIndex < numOfClusters; ++centroidIndex)
        printf("%d, ", centroids[centroidIndex]);
    printf("\n");
}

void KMeansClustering::freeClusters() {
//    printf("Start Freeing\n");
    delete[] centroids;
    delete[] visited;

//    for (int i = 0; i < numOfClusters; ++i) {
//        delete[] clusters[i]->customers;
//        delete clusters[i];
//    }
//    delete[] clusters;
//    printf("End Freeing");
}



