//
// Created by wmw13 on 05/03/2021.
//

#ifndef TESTSAMPLECODE_CLUSTERER_H
#define TESTSAMPLECODE_CLUSTERER_H
#include "../LocalSearchAndGenerateTour/LocalSearches.h"

class Clusterer {
public:
    struct Node{
        int* customers;
        double distance;
        int sizeOfCluster;
    };
    static int k;
    static struct Node** clusters;
    static int * getKNN(int, int, const bool*);

    static void optimiseClusters();
    static int numOfClusters;
    static void freeClusters();
    static double getClosestDistance(int, int);
    static void displayClusters();
    static void createClusters(int);
    static int* getRouteFromClusters(int*);
    static double getRouteLength(int*);
};


#endif //TESTSAMPLECODE_CLUSTERER_H
