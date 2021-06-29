#ifndef TESTSAMPLECODE_LOCALSEARCHES_H
#define TESTSAMPLECODE_LOCALSEARCHES_H

#include "../LocalSearchAndGenerateTour/GenerateTour.h"
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>
#include <climits>

class localSearch{
private:
    int randomSearchIteration;
    int twoOptIterations;
    std::map<std::string, int> localSearchPheromone;
    std::map<std::string, int> localSearchPheromoneCluster;
    int findNearestCustomer(int customer);
public:
    localSearch(int RandomSearchIteration, int TwoOptIterations);
    virtual ~localSearch();
    int getTotalWeight();
    int getTotalWeightCluster();
    void randomPheromoneLocalSearch(int* bestRoute);
    void randomPheromoneLocalSearchWithTwoOpt(int* bestRoute);
    void randomPheromoneLocalSearchWithTwoOptCluster(int* bestRoute);
    void randomLocalSearch(int* bestRoute);
    void decreaseLocalSearchPheromone();
    void decreaseLocalSearchPheromoneCluster();
    void twoOptLocalPheromoneAddonSearch(int* currentRoute);
    std::vector<int> getRandomNumber();
    std::vector<int> getRandomNumberCluster();
    void twoOptSwap(int i , int j, int* route, const int* currRoute);
    void twoOptLocalSearch(int* bestRoute);
    void LKSearch(int* bestRoute);

};
#endif //TESTSAMPLECODE_LOCALSEARCHES_H
