//
// Created by wmw13 on 09/03/2021.
//

#include "Cluster.h"

void Cluster::Node::getTotalDemand() {
    double total = 0.0;
    for (int i = 0; i < sizeOfCluster; ++i)
        total += get_customer_demand(customers[i]);
    demand = total;
}

std::string Cluster::Node::displayNode() {
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

void Cluster::Node::getTotalDistance() {
    double length = 0.0;

    for (int customerIndex = 0; customerIndex < sizeOfCluster - 1; ++customerIndex)
        length += customers[customerIndex];

    distance = length;
}

Cluster::Cluster() {
    createClusters();
}

Cluster::~Cluster(){
    for (auto a : *centroids)
        delete a;

    for (auto a : *clusters) {
        delete[] a->customers;
        delete a;
    }
    delete centroids;
    delete clusters;
    delete[] visited;
}

/*
 * Random initial centroid selection.
 */
//void Cluster::initialCentroids(){
//    for (int centroidIndex = 0; centroidIndex < numOfClusters; ++centroidIndex) {
//        //Loops until it finds a viable centroid.
//        node n;
//        do {
//            n = getNodeInfo((rand() % NUM_OF_CUSTOMERS-1)+1);
//        } while (visited[n.id]);
//        //Adds the centroid to the visited array.
//        auto temp = new std::pair<double,double>();
//        temp->first = n.x;
//        temp->second = n.y;
//        centroids->push_back(temp);
//        visited[n.id] = true;
//    }
//}

void Cluster::initialCentroids(){
    int spaces = NUM_OF_CUSTOMERS/numOfClusters;
    for (int i = spaces-1; i < NUM_OF_CUSTOMERS; i+= spaces) {
        node n = getNodeInfo(i);
        auto temp = new std::pair<double,double>();
        temp->first = n.x;
        temp->second = n.y;
        centroids->push_back(temp);
        visited[n.id] = true;
    }
}

void Cluster::displayCentroids(){
    for (int centroidIndex = 0; centroidIndex < numOfClusters; ++centroidIndex) {
        printf("%d :: %f, %f\n",centroidIndex+1,centroids->at(centroidIndex)->first,centroids->at(centroidIndex)->second);
    }printf("\n");
}

double Cluster::getXYDistance(double x, double y, int centroidNum){
    return sqrt(pow((x-centroids->at(centroidNum)->first),2)+(pow((y-centroids->at(centroidNum)->second),2)));
}

void Cluster::clusterAroundCentroids() {
    for (int customerIndex = 1; customerIndex <=NUM_OF_CUSTOMERS; ++customerIndex) {
        double minDis = INT_MAX;
        int bestCentoidIndex = -1;
        node n = getNodeInfo(customerIndex);
        for (int centroidIndex = 0; centroidIndex < numOfClusters; ++centroidIndex) {
            if(clusters->at(centroidIndex)->demand < MAX_CAPACITY) {
                double tempDis = getXYDistance(n.x, n.y, centroidIndex);
//            printf("%d,%d,%f:%f\n",customerIndex,centroidIndex,tempDis,minDis);
                if (tempDis < minDis) {
                    minDis = tempDis;
                    bestCentoidIndex = centroidIndex;
                }
            }
        }
        clusters->at(bestCentoidIndex)->customers[clusters->at(bestCentoidIndex)->sizeOfCluster++] = customerIndex;
        clusters->at(bestCentoidIndex)->getTotalDemand();
    }
}

bool Cluster::generateCentroidsBasedOnClusters(){
    bool change = true;
    for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex) {
        auto xMM = std::pair<double,double>();
        auto yMM = std::pair<double,double>();
        //first is MIN. second is MAX.
        xMM.first = INT_MAX; xMM.second = 0;
        yMM.first = INT_MAX; yMM.second = 0;
        for (int customerIndex = 0; customerIndex < clusters->at(clusterIndex)->sizeOfCluster; ++customerIndex) {
            node n = getNodeInfo(clusters->at(clusterIndex)->customers[customerIndex]);
            if(n.x < xMM.first)
                xMM.first = n.x;
            if(n.x > xMM.second)
                xMM.second = n.x;
            if(n.y < yMM.first)
                yMM.first = n.y;
            if(n.y > yMM.second)
                yMM.second = n.y;
        }
        double valx = (xMM.first + ((xMM.second-xMM.first)*0.5));
        double valy = (yMM.first + ((yMM.second-yMM.first)*0.5));
        if(centroids->at(clusterIndex)->first == valx && centroids->at(clusterIndex)->second == valy)
            change = false;
        centroids->at(clusterIndex)->first = valx;
        centroids->at(clusterIndex)->second = valy;
        clusters->at(clusterIndex)->sizeOfCluster = 0;
        clusters->at(clusterIndex)->demand = 0;
    }
    return change;
}

void Cluster::displayClusters() {
    for (auto c : *clusters) {
        printf("%s\n",c->displayNode().c_str());
    }
}

void Cluster::createClusters() {
    numOfClusters = MIN_VEHICLES;
    clusters = new std::vector<struct Node *>;
    for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex) {
        clusters->push_back(new struct Node);
        clusters->at(clusterIndex)->customers = new int[NUM_OF_CUSTOMERS];
        clusters->at(clusterIndex)->sizeOfCluster = 0;
        clusters->at(clusterIndex)->distance = 0;
        clusters->at(clusterIndex)->demand = 0;
        for (int customerIndex = 0; customerIndex < NUM_OF_CUSTOMERS; ++customerIndex)
            clusters->at(clusterIndex)->customers[customerIndex] = -1;
    }
    centroids = new std::vector<std::pair<double,double>*>();
    visited = new bool[NUM_OF_CUSTOMERS + 1];
    for (int customerIndex = 0; customerIndex <= NUM_OF_CUSTOMERS; ++customerIndex)
        visited[customerIndex] = false;

    initialCentroids();
//    displayCentroids();
    //LOOP
    int changeUB = 2;
    int changeCounter = 0;
    while (changeCounter < changeUB){
//    for (int generations = 1; generations <= 4; ++generations) {
//        displayCentroids();
        clusterAroundCentroids();
        if(generateCentroidsBasedOnClusters())
            changeCounter = 0;
        changeCounter++;
    }
    clusterAroundCentroids();
//    displayClusters();
}
