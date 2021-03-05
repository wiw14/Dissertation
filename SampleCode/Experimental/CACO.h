//
// Created by wmw13 on 05/03/2021.
//

#ifndef TESTSAMPLECODE_CACO_H
#define TESTSAMPLECODE_CACO_H
#include "Clusterer.h"

class CACO {
private:
    std::map<std::string,double>::iterator iterator;
    std::map<std::string, double> pheromones;
    int ** routes;
    int *bestRoute;
    double pheromoneDecrease, Q, bestRouteLength,alpha,beta;
    double ** probability;
    int numOfAnts, probabilitySize;
    localSearch* LS;

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
    CACO(int,double,double,int,double,double,int,int);
    virtual ~CACO ();
    void optimize (int);
    int* returnResults();
    double getRL(int* route);
};


#endif //TESTSAMPLECODE_CACO_H
