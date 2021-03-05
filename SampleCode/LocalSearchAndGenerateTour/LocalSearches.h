//
// Created by wmw13 on 16/02/2021.
//

#ifndef TESTSAMPLECODE_LOCALSEARCHES_H
#define TESTSAMPLECODE_LOCALSEARCHES_H

#include "../LocalSearchAndGenerateTour/GenerateTour.h"

class localSearch{
private:
    int randomSearchIteration;
    int twoOptIterations;
    std::map<std::string, int> localSearchPheromone;
public:
    localSearch(int RandomSearchIteration, int TwoOptIterations);
    virtual ~localSearch();
    int getTotalWeight();
    void randomPheromoneLocalSearch(int* bestRoute);
    void randomPheromoneLocalSearchWithTwoOpt(int* bestRoute);
    void randomLocalSearch(int* bestRoute);
    void decreaseLocalSearchPheromone();
    void twoOptLocalPheromoneAddonSearch(int* currentRoute);
    std::vector<int> getRandomNumber();
    void twoOptSwap(int i , int j, int* route, const int* currRoute);
    void twoOptLocalSearch(int* bestRoute);
    void LKSearch(int* bestRoute);

};
#endif //TESTSAMPLECODE_LOCALSEARCHES_H
