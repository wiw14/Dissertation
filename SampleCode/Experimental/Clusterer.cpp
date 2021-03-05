//
// Created by wmw13 on 05/03/2021.
//

#include <climits>
#include "Clusterer.h"

int Clusterer::k;
struct Clusterer::Node** Clusterer::clusters;
int Clusterer::numOfClusters;

double Clusterer::getRouteLength(int* clusterRoute) {
    double length = 0.0;
    for (int i = 1; i < numOfClusters; ++i) {
        length+=getClosestDistance(clusterRoute[i-1],clusterRoute[i]);
    }
    return length;
}

void Clusterer::freeClusters() {
    for (int i = 0; i < numOfClusters; ++i) {
        delete[] clusters[i]->customers;
        delete clusters[i];
    }
    delete[] clusters;
}

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
            numOfClusters++;
        }
    }
    delete[] visited;
}

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

int* Clusterer::getRouteFromClusters(int* clusterRoute){
    int* route = new int[NUM_OF_CUSTOMERS+1], counter = 0;
    for (int i = 0; i < numOfClusters; ++i) {
        for (int j = 0; j < clusters[clusterRoute[i]]->sizeOfCluster; ++j) {
            route[counter++] = clusters[clusterRoute[i]]->customers[j];
        }
    }
    delete[] clusterRoute;
    return route;
}