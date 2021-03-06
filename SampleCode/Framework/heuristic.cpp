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
#include "../Experimental/CACOHeuristic.h"
#include "../Experimental/Clusterer.h"
#include "../ChainedLKSearch/CLKSearchHeuristic.h"
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

    ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();

    //printf("time %ld\n",ms);

    Clusterer::createClusters(4);
}


/*
 * List of all the currently implemented heuristics.
 * Activate heuristics by uncommenting.
 */
void run_heuristic() {
    /*
    * Shortest Path Algorithms.
    */
    greedyHeuristic();
    //randomHeuristic();
//    DijkstrasHeuristic();
    //KNNHeuristic();
    //CLKSearchHeuristic();

    /*
     * Evolutionary Algorithms
     */
    //GAHeuristic();
    //ACOHeuristic();
    //ACOCSHeuristic();
    //MMACOHeuristic();

    //CACOHeuristic();
}

/*
 * Calculates the time the heuristic executed for.
 */
void end_heuristic(){
    ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count() - ms;

    printf("time %ld milliseconds\n",ms);
    printf("time %f seconds\n",((double)ms/(double)1000));
    printf("\n");
}

/*
 * free memory used by the tour array.
 */
void free_heuristic() {
    Clusterer::freeClusters();

    delete[] best_sol->tour;
    delete best_sol;
}

