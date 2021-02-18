//
// Created by wmw13 on 16/02/2021.
//

#ifndef TESTSAMPLECODE_LOCALSEARCHES_H
#define TESTSAMPLECODE_LOCALSEARCHES_H

#include "EVRP.hpp"
#include "heuristic.hpp"
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>

class localSearch{
private:
    int randomSearchIteration;
    int twoOptIterations;
    std::map<std::string, int> localSearchPheromone;
    static int findClosestChargingStation(int customer);
    double getTotalEnergyConsumption(int* route, int startCustomer,int upperBound);
    int getTotalLoad(int* route, int startCustomer);
    int* findOptimalCS(int* subRoute, int size);
    static double getTotalDistance(int*route,int size);
    bool getIsValidEnergy(int*route,int size);

public:
    localSearch(int RandomSearchIteration, int TwoOptIterations);
    virtual ~localSearch();
    double getRouteLength(const int *route);
    int getTotalWeight();
    void randomPheromoneLocalSearch(int* bestRoute);
    void randomPheromoneLocalSearchWithTwoOpt(int* bestRoute);
    void randomLocalSearch(int* bestRoute);
    void decreaseLocalSearchPheromone();
    void twoOptLocalPheromoneAddonSearch(int* currentRoute);
    std::vector<int> getRandomNumber();
    std::string getArcCode(int customerA, int customerB);
    void twoOptSwap(int i , int j, int* route, const int* currRoute);
    void twoOptLocalSearch(int* bestRoute);
};
#endif //TESTSAMPLECODE_LOCALSEARCHES_H
