#include "KMeansClustering.h"

/*
 * NODE FUNCTIONS.
 */
/*
 * Displays the customers within a node.
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

/*
 * Gets the total distance within a cluster.
 */
void KMeansClustering::Node::getTotalDistance() {
    double length = 0.0;

    for (int customerIndex = 0; customerIndex < sizeOfCluster - 1; ++customerIndex)
        length += customers[customerIndex];

    distance = length;
}

/*
 * BODY OF CLASS.
 */
//Variables to configure the clusters.
int KMeansClustering::numOfClusters;
int *KMeansClustering::centroids;
bool *KMeansClustering::visited;
struct KMeansClustering::Node **KMeansClustering::clusters;

/*
 * Randomly selects  k number of initial centroids to generate clusters from.
 */
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

/*
 * Instantiates the variables for the clusters.
 */
void KMeansClustering::createClusters(int k) {
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

    initialCentroids();
//    displayCentroids();
    clusterAroundCentroids();
}

/*
 * Swaps two clusters for cluster local search.
 */
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

/*
 * Two-opt local search for clusters.
 */
//void KMeansClustering::optimiseClusters(struct Node* node){
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
//
//    //De-allocates the memory used above.
//    //delete[] tempRoute;
//}

/*
 * Generates clusters from the centroids.
 */
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

}

/*
 * Gets the route length of a set of clusters.
 */
double KMeansClustering::getRouteLength(int *clusterRoute) {
    double length = 0.0;
    for (int i = 0; i < numOfClusters; ++i)
        length += clusters[clusterRoute[i]]->distance;
    return length;
}

/*
 * Gets the closest distance between two clusters.
 */
double KMeansClustering::getClosestDistance(int posA, int posB) {
    double minDist = INT_MAX;
    for (int i = 0; i < clusters[posA]->sizeOfCluster; ++i) {
        for (int j = 0; j < clusters[posB]->sizeOfCluster; ++j) {
            double currentDist = get_distance(clusters[posA]->customers[i], clusters[posB]->customers[j]);
            if (currentDist < minDist)
                minDist = currentDist;
        }

    }
    return minDist;
}

/*
 * Creates a route of customers from the clusters.
 */
int *KMeansClustering::getRouteFromClusters(const int *clusterRoute) {
    int *route = new int[NUM_OF_CUSTOMERS + 1], counter = 0;
    for (int i = 0; i < numOfClusters; ++i) {
        int *tempClusterRoute = clusters[clusterRoute[i]]->customers;
        for (int j = 0; j < clusters[clusterRoute[i]]->sizeOfCluster; ++j) {
            route[counter++] = tempClusterRoute[j];
        }
    }
    return route;
}

/*
 * Display all the centroids for debugging.
 */
void KMeansClustering::displayCentroids() {
    printf("numOfClusters=%d : ", numOfClusters);
    for (int centroidIndex = 0; centroidIndex < numOfClusters; ++centroidIndex)
        printf("%d, ", centroids[centroidIndex]);
    printf("\n");
}

void KMeansClustering::freeClusters() {
    delete[] centroids;
    delete[] visited;

//    for (int i = 0; i < numOfClusters; ++i) {
//        delete[] clusters[i]->customers;
//        delete clusters[i];
//    }
//    delete[] clusters;
}



