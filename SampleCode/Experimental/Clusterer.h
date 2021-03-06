//
// Created by wmw13 on 05/03/2021.
//

#ifndef TESTSAMPLECODE_CLUSTERER_H
#define TESTSAMPLECODE_CLUSTERER_H
#include "../LocalSearchAndGenerateTour/LocalSearches.h"

class Clusterer {
private:
    //static void twoOptSwap(int,int,int*,const int*);
    static int twoOptIterations;
public:
    struct Node{
        int* customers;
        double distance;
        int sizeOfCluster;
    };
    static int k;
    static struct Node** clusters;
    static int * getKNN(int, int, const bool*);

    static void optimiseCluster(struct Node*);
    static int numOfClusters;
    static void freeClusters();
    static double getClosestDistance(int, int);
    static void displayClusters();
    static void createClusters(int);
    static int* getRouteFromClusters(int*);
    static double getRouteLength(int*);
    static double calculateClusterDistance(int*,int);
};


#endif //TESTSAMPLECODE_CLUSTERER_H
