//
// Created by wmw13 on 11/02/2021.
//

#ifndef TESTSAMPLECODE_ACO_H
#define TESTSAMPLECODE_ACO_H

#include "LocalSearches.h"
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>


class ACO {
private:
    std::map<std::string,double>::iterator iterator;
    std::map<std::string, double> pheromones;
    int ** routes;
    int* bestRoute;
    double pheromoneDecrease, Q, bestRouteLength,alpha,beta;
    double ** probability;
    int numOfAnts, probabilitySize;
    localSearch* LS;

    std::default_random_engine seed;
    std::uniform_real_distribution<double> distribution;

    static std::string getArcCode(int,int);
    void resetRoute(int);
    void resetProbability();
    void updatePheromones ();
    void route(int);
    bool visited(int,int);
    bool valid(int);
    static bool exists (int, int);
    double length(int);
    double amountOfPheromone(double) const;
    double getProbability(int,int,int);
    int getNextCustomer();

public:
    ACO(int,double,double,int,double,double,int,int);
    virtual ~ACO ();
    void optimize (int);
    int* returnResults();
    double getRL(int* route);
};


#endif //TESTSAMPLECODE_ACO_H
