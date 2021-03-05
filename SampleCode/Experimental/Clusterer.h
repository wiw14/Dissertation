//
// Created by wmw13 on 05/03/2021.
//

#ifndef TESTSAMPLECODE_CLUSTERER_H
#define TESTSAMPLECODE_CLUSTERER_H
#include "../LocalSearchAndGenerateTour/LocalSearches.h"

class Clusterer {
private:
    struct Node{
        int* customers;
        double distance;
        int sizeOfCluster;
    };
    int k;
    struct Node** clusters;
    static int * getKNN(int, int, const bool*);

public:
    int numOfClusters;
    Clusterer(int);
    ~Clusterer();
    double getClosestDistance(int, int);
    void displayClusters();
    void createClusters();
    int* getRouteFromClusters(int*);
};


#endif //TESTSAMPLECODE_CLUSTERER_H
