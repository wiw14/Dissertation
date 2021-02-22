//
// Created by wmw13 on 22/02/2021.
//

#ifndef TESTSAMPLECODE_MAXMINACO_H
#define TESTSAMPLECODE_MAXMINACO_H

#include "LocalSearches.h"
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>

class MaxMinACO {
private:
    std::map<std::string,double>::iterator iterator;
    std::map<std::string, double> pheromones;
    int ** routes;
    int *bestRoute;
    double pheromoneDecrease, Q, bestRouteLength,alpha,beta, tMAX,tMIN, pBest;
    double ** probability;
    int numOfAnts, probabilitySize;
    localSearch* LS;

    std::default_random_engine seed;
    std::uniform_real_distribution<double> distribution;

    static std::string getArcCode(int,int);
    void resetRoute(int);
    void resetProbability();
    void updatePheromones (int, double, int);
    void route(int);
    bool visited(int,int);
    bool valid(int);
    static bool exists (int, int);
    double length(int);
    double amountOfPheromone(double) const;
    double getProbability(int,int,int);
    int getNextCustomer();
    void instantiatePheromones();
    void calculateTMAX(double);
    void calculateTMIN();

public:
    MaxMinACO(int,double,double,int,double,double,int,int);
    virtual ~MaxMinACO ();
    void optimize (int);
    int* returnResults();
    double getRL(int*);
};


#endif //TESTSAMPLECODE_MAXMINACO_H
