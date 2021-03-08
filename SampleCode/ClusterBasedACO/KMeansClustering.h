//
// Created by wmw13 on 08/03/2021.
//

#ifndef TESTSAMPLECODE_KMEANSCLUSTERING_H
#define TESTSAMPLECODE_KMEANSCLUSTERING_H

#include <string>
#include "../LocalSearchAndGenerateTour/LocalSearches.h"

class KMeansClustering {
private:
    static int* centroids;
    static bool* visited;
//    static void twoOptSwap(int,int,int*,const int*);
    static int twoOptIterations;

public:
    struct Node{
        int* customers;
        double distance;
        int sizeOfCluster;
        std::string displayNode();
        void getTotalDistance();
    };
    static int numOfClusters;
    static struct Node** clusters;

    static double getClosestDistance(int,int);
    static double getRouteLength(int*);
    static void createClusters(int);
//    static void optimiseClusters(struct Node*);
//        static double calculateClusterDistance(int*,int);
    static void initialCentroids(void);
    static void freeClusters(void);
    static int* getRouteFromClusters(const int*);
    static void clusterAroundCentroids(void);

    //DEBUGGING
    static void displayCentroids(void);
};


#endif //TESTSAMPLECODE_KMEANSCLUSTERING_H
