//
// Created by wmw13 on 05/03/2021.
//

#ifndef TESTSAMPLECODE_GENERATETOUR_H
#define TESTSAMPLECODE_GENERATETOUR_H

#include "../Framework/EVRP.hpp"
#include "../Framework/heuristic.hpp"
#include "../ClusterBasedACO//KMeansClustering.h"
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>
#include <climits>

class GenerateTour {
private:
    static int findClosestChargingStation(int customer);
    static double getTotalDistance(int*route,int size);
    static bool getIsValidEnergy(int*route,int size);
    static bool getIsValidCapacity(int *route, int size);
    static double getTotalLoadWithAddedDepot(int* route,int depotPos);
    static bool checkAllCustomersVisited(int* tour,int size);
public:
    static double getRouteLengthQuick(const int* route);
    static double getRouteLength(const int *route);
    static std::string getArcCode(int customerA, int customerB);
    static bool checkSolution(int* tour, int size);
    static double getBasicLength(int* bestRoute);
};


#endif //TESTSAMPLECODE_GENERATETOUR_H
