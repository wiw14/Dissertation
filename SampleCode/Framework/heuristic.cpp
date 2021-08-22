#include<cmath>
#include<string>
#include<limits.h>

//#include "EVRP.hpp"
#include "../GreedyEuclidean/greedyEuclideanHeuristic.h"
#include "../RandomHeuristic/randomHeuristic.h"
#include "../AntColonyOptimisation/ACOHeuristic.h"
#include "../Dijkstra/DijkstrasHeuristic.h"
#include "../KNN/KNNHeuristic.h"
#include "../GeneticAlgorithm/GAHeuristic.h"
#include "../ExternalClusteringACO/CACOHeuristic.h"
#include "../ExternalClusteringACO/Clusterer.h"
#include "../ExternalClusteringACO/KMeansClustering.h"
#include "../InternalClusteringACO/ACOOnClustersHeuristic.h"
//#include "heuristic.hpp"
#include <chrono>

using namespace std;

solution *best_sol;
long int ms;

/*
 * Initialize the structure of your heuristic in this function.
 * Saves the current time for processing time analysis.
 */
void initialize_heuristic() {
    best_sol = new solution;
    best_sol->tour = new int[NUM_OF_CUSTOMERS + 1000];
    best_sol->id = 1;
    best_sol->steps = 0;
    best_sol->tour_length = INT_MAX;

    //Gets the time the heuristic is started.
    ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();

    //Generates clusters from the input file used for external clustering ACO.
    //Needs to be uncommented for external clustering.
//    KMeansClustering::createClusters(4);
}

/*
 * Used to validate a route.
 */
void checkRouteIsValid(int* route, int routeSize){
    check_solution(route,routeSize);
}


/*
 * List of all the currently implemented heuristics.
 * Activate heuristics by uncommenting.
 */
void run_heuristic(int input,vector<double>*vars) {
    if(input == 1) //Greedy heuristic.
        greedyHeuristic((int)vars->at(0), (int)vars->at(1));
    else if(input == 2)//Random heuristic.
        randomHeuristic((int)vars->at(0), (int)vars->at(1));
    else if(input == 3)//Dijkstra heuristic.
        DijkstrasHeuristic((int)vars->at(0), (int)vars->at(1));
    else if(input == 4)//KNN heuristic.
        KNNHeuristic((int)vars->at(0), (int)vars->at(1), (int)vars->at(2));
    else if(input == 5)//Internal Clustering ACO.
        ACOOnClustersHeuristic((int)vars->at(0), (int)vars->at(1), (int)vars->at(2),vars->at(3),vars->at(4),vars->at(5),vars->at(6),(int)vars->at(7), (int)vars->at(8));
    else if(input == 6)//Genetic Algorithm.
        GAHeuristic((int)vars->at(0), (int)vars->at(1), (int)vars->at(2), (int)vars->at(6), (int)vars->at(7),(int)vars->at(3),(int)vars->at(4),(int)vars->at(5));
    else if(input == 7)//Ant Colony Optimisation.
        ACOHeuristic((int)vars->at(0), (int)vars->at(1), (int)vars->at(2),vars->at(3),vars->at(4),vars->at(5),vars->at(6),(int)vars->at(7), (int)vars->at(8));
    else if(input == 8)//Ant Colony Optimisation with Charging Stations.
        ACOCSHeuristic((int)vars->at(0), (int)vars->at(1), (int)vars->at(2),vars->at(3),vars->at(4),vars->at(5),vars->at(6),(int)vars->at(7), (int)vars->at(8));
    else if(input == 9)//Max Min Ant Colony Optimisation.
        MMACOHeuristic((int)vars->at(0), (int)vars->at(1), (int)vars->at(2),vars->at(3),vars->at(4),vars->at(5),vars->at(6),(int)vars->at(8), (int)vars->at(9),(double)vars->at(7));
    else if(input == 10)//External Clustering ACO.
        CACOHeuristic((int)vars->at(0), (int)vars->at(1), (int)vars->at(2),vars->at(3),vars->at(4),vars->at(5),vars->at(6),(int)vars->at(7), (int)vars->at(8));

}

/*
 * Calculates the time the heuristic executed for.
 */
double end_heuristic(){
    ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count() - ms;

    //Displays the best route from the metaheuristic.
    for (int i = 0; i < best_sol->steps; ++i)
        printf("%d, ",best_sol->tour[i]);
    printf("\n");

    /*
     * Displays the time in seconds.
     */
    printf("time %f seconds\n",((double)ms/(double)1000));
    printf("\n");

    /*
    * Frees the generated clusters for external clustering ACO.
    */
    //Clustering.
    //Needs to be uncommented for external clustering.
//    KMeansClustering::freeClusters();

    //Returns the run time in seconds.
    return ((double)ms/(double)1000);
}

/*
 * free memory used by the tour array.
 */
void free_heuristic() {

    delete[] best_sol->tour;
    delete best_sol;
}

