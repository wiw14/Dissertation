#ifndef TESTSAMPLECODE_CLUSTERACO_H
#define TESTSAMPLECODE_CLUSTERACO_H
#include "Cluster.h"

class ClusterACO {
private:
    std::map<std::string, double> pheromones;
    int ** routes;
    int *bestRoute;
    int* alphabet;
    int sizeOfAlphabet;
    double pheromoneDecrease, Q, bestRouteLength,alpha,beta;
    double ** probability;
    int numOfAnts, probabilitySize;

    std::default_random_engine seed;
    std::uniform_real_distribution<double> distribution;

    static std::string getArcCode(int,int);
    void resetRoute(int);
    void resetProbability();
    void updatePheromones (int,int);
    void route(int);
    bool visited(int,int);
    bool valid(int);
    bool exists (int, int);
    double length(int);
    double amountOfPheromone(double) const;
    double getProbability(int,int,int);
    int getNextCustomer();


public:
    ClusterACO(int,double,double,int,double,double,int,int,int*, int);
    virtual ~ClusterACO ();
    void optimize (int);
    int* returnResults();
    double getRL(int* route);
};


#endif //TESTSAMPLECODE_CLUSTERACO_H
