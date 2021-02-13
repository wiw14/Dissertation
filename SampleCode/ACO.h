//
// Created by wmw13 on 11/02/2021.
//

#ifndef TESTSAMPLECODE_ACO_H
#define TESTSAMPLECODE_ACO_H

#include "EVRP.hpp"
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
    std::map<std::string, int> localSearchPheromone;

    std::default_random_engine seed;
    std::uniform_real_distribution<double> distribution;

    static char* getArcCode(int,int);
    void printPheromones();
    void resetRoute(int);
    void resetProbability();
    void randomPheromoneLocalSearch();
    void randomLocalSearch();
    void twoOptLocalSearch();
    void updatePheromones ();
    void route(int);
    void decreaseLocalSearchPheromone();
    void printLocalSearchPheromones();
    bool visited(int,int);
    bool valid(int);
    bool exists (int, int);
    double length(int);
    double amountOfPheromone(double,int,int);
    double getProbability(int,int,int);
    double getRouteLength(int*);
    int getNextCustomer();
    std::vector<int> getRandomNumber();
    int getTotalWeight();

public:
    ACO(int,double,double,int,double,double);
    virtual ~ACO ();
    void optimize (int);
    int* returnResults();
};


#endif //TESTSAMPLECODE_ACO_H