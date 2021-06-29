#ifndef TESTSAMPLECODE_MAXMINACO_H
#define TESTSAMPLECODE_MAXMINACO_H

#include "../LocalSearchAndGenerateTour/LocalSearches.h"
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>

class MaxMinACO {
private:
    //Map to store pheromones throughout the pheromones.
    std::map<std::string, double> pheromones;
    //Stores the route for each ant at each iteration.
    int ** routes;
    //Stores the best route throughout the ACO.
    int *bestRoute;
    //Required variables for the ACO.
    double pheromoneDecrease, Q, bestRouteLength,alpha,beta, tMAX,tMIN, pBest;
    //Used to temporarily store probabilities.
    double ** probability;
    //Size of the ACO.
    int numOfAnts, probabilitySize;
    //Local Search.
    localSearch* LS;

    //Random number generators.
    std::default_random_engine seed;
    std::uniform_real_distribution<double> distribution;

    //Gets the arc code (string) from two nodes to search the pheromone map.
    static std::string getArcCode(int,int);
    //Resets the route for an ant.
    void resetRoute(int);
    //Resets the temporary probability array.
    void resetProbability();
    //Updates the pheromones based on the ants route.
    void updatePheromones (int, double, int);
    //Generates the route for the ant.
    void route(int);
    //Checks whether a customer for an ant has been visited.
    bool visited(int,int);
    //Checks whether an ant's route is valid.
    bool valid(int);
    //Whether an edge exists between two customers.
    //Used to validate routes, for the EVRP there are edges between all nodes.
    static bool exists (int, int);
    //Gets the total length of the ants route.
    double length(int);
    //Calculates the amount of pheromone for the ant.
    double amountOfPheromone(double) const;
    //Calculates the probability for selecting a customer.
    double getProbability(int,int,int);
    //Chooses the next customer based on the probability.
    int getNextCustomer();
    //Instantiates the pheromones to pheromone upper limit.
    void instantiatePheromones();
    //Calculates a new pheromone maximum limit.
    void calculateTMAX(double);
    //Calculates a new pheromone minimum limit.
    void calculateTMIN();
    //Displays the pheromones for debugging.
    void displayPheromones();

public:
    //Constructor.
    MaxMinACO(int,double,double,int,double,double,double,int,int);
    //De-constructor.
    virtual ~MaxMinACO ();
    //Locates a local optimum through ant generation and local search.
    void optimize (int);
    //Returns the best route from the ACO.
    int* returnResults();
    //Returns the route length of the best route using GenerateTour().
    double getRL(int*);
};


#endif //TESTSAMPLECODE_MAXMINACO_H
