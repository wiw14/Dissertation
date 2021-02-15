//
// Created by wmw13 on 15/02/2021.
//

#ifndef TESTSAMPLECODE_ACOWITHCHARGINGSTATION_H
#define TESTSAMPLECODE_ACOWITHCHARGINGSTATION_H

//
// Created by wmw13 on 11/02/2021.
//

#include "ACO.h"
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>


class ACOCS {
private:
    std::map<std::string,double>::iterator iterator;
    std::map<std::string, double> pheromones;
    int ** routes;
    int* bestRoute;
    double pheromoneDecrease, Q, bestRouteLength,alpha,beta;
    double ** probability;
    int numOfAnts, probabilitySize, twoOptIterations, randomSearchIteration;
    std::map<std::string, int> localSearchPheromone;

    std::default_random_engine seed;
    std::uniform_real_distribution<double> distribution;

    static std::string getArcCode(int,int);
    void resetRoute(int);
    void resetProbability();
    void randomPheromoneLocalSearch();
    void randomLocalSearch();
    void twoOptLocalSearch();
    void updatePheromones ();
    void route(int);
    void decreaseLocalSearchPheromone();
    void twoOptLocalPheromoneAddonSearch(int* );
    static void twoOptSwap(int , int , int*,const int*);
    bool visited(int,int);
    bool valid(int);
    static bool exists (int, int);
    double length(int);
    double amountOfPheromone(double) const;
    double getProbability(int,int,int);
    int getNextCustomer();
    std::vector<int> getRandomNumber();
    int getTotalWeight();

public:
    ACOCS(int,double,double,int,double,double,int,int);
    virtual ~ACOCS ();
    void optimize (int);
    int* returnResults();
    static double getRouteLength(const int*);
};



#endif //TESTSAMPLECODE_ACOWITHCHARGINGSTATION_H
