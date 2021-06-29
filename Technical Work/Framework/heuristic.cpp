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
void run_heuristic(int input) {
    if(input == 1)//Greedy heuristic.
        greedyHeuristic();
    else if(input == 2)//Random heuristic.
        randomHeuristic();
    else if(input == 3)//Dijkstra heuristic.
        DijkstrasHeuristic();
    else if(input == 4)//KNN heuristic.
        KNNHeuristic();
    else if(input == 5)//Internal Clustering ACO.
        ACOOnClustersHeuristic();
    else if(input == 6)//Genetic Algorithm.
        GAHeuristic();
    else if(input == 7)//Ant Colony Optimisation.
        ACOHeuristic();
    else if(input == 8)//Ant Colony Optimisation with Charging Stations.
        ACOCSHeuristic();
    else if(input == 9)//Max Min Ant Colony Optimisation.
        MMACOHeuristic();
    else if(input == 10)//External Clustering ACO.
        CACOHeuristic();

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

