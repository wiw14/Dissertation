#ifndef TESTSAMPLECODE_KMEANSCLUSTERING_H
#define TESTSAMPLECODE_KMEANSCLUSTERING_H

#include <string>
#include "../LocalSearchAndGenerateTour/LocalSearches.h"

class KMeansClustering {
private:
    static int* centroids;
    static bool* visited;

public:
    //Struct used to represent a cluster.
    struct Node{
        int* customers;
        double distance;
        int sizeOfCluster;
        std::string displayNode();
        void getTotalDistance();
    };

    //Number of clusters (k)
    static int numOfClusters;
    //Array of clusters.
    static struct Node** clusters;

    //Gets the smallest distance between two clusters.
    static double getClosestDistance(int,int);
    //Gets the route length of all the customers.
    static double getRouteLength(int*);
    //Instantiates the variables used for clustering
    static void createClusters(int);

    //Local search for clustering
//  static void optimiseClusters(struct Node*);
//  static double calculateClusterDistance(int*,int);

    //Randomly selects centroids for clustering.
    static void initialCentroids(void);
    //Frees the variables used in clustering.
    static void freeClusters(void);
    //Gets a route length from the clusters.
    static int* getRouteFromClusters(const int*);
    //Generates clusters from the centroids.
    static void clusterAroundCentroids(void);

    //Displays all the centroids for debugging.
    static void displayCentroids(void);
};


#endif //TESTSAMPLECODE_KMEANSCLUSTERING_H
