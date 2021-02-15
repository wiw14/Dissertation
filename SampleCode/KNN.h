//
// Created by wmw13 on 15/02/2021.
//

#ifndef TESTSAMPLECODE_KNN_H
#define TESTSAMPLECODE_KNN_H
#include "EVRP.hpp"
#include "heuristic.hpp"
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>
#include <climits>
class KNN {
private:
    int *v;
    int*filter;
    int **neighbours;
    int randomSearchIteration,twoOptIterations;
    std::map<std::string, int> localSearchPheromone;

    void randomPheromoneLocalSearch();
    void decreaseLocalSearchPheromone();
    int getTotalWeight();
    std::vector<int> getRandomNumber();
    void randomLocalSearch();
    void twoOptLocalPheromoneAddonSearch(int* currentRoute);
    void twoOptSwap(int i , int j, int* route, const int* currRoute);
    void twoOptLocalSearch();
    std::string getArcCode(int customerA, int customerB);

public:

    void generateRoute(int);
    int findNextClosestNode(int);
    double getRouteLength(const int *);
    KNN(int);
    int *getKNN(int, int);
    void printRoute();
    ~KNN();
};


#endif //TESTSAMPLECODE_KNN_H
