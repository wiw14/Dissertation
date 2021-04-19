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
#include "../ClusterBasedACO//CACOHeuristic.h"
#include "../ClusterBasedACO//Clusterer.h"
#include "../ClusterBasedACO//KMeansClustering.h"
#include "../Experimental/ACOOnClustersHeuristic.h"
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

    //Generates clusters from the input file.
    //Clusterer::createClusters(4);
//    KMeansClustering::createClusters(4);
}

void checkRouteIsValid(int* route, int routeSize){
    check_solution(route,routeSize);
}


/*
 * List of all the currently implemented heuristics.
 * Activate heuristics by uncommenting.
 */
void run_heuristic(int input) {
    if(input == 1)
        greedyHeuristic();
    else if(input == 2)
        randomHeuristic();
    else if(input == 3)
        DijkstrasHeuristic();
    else if(input == 4)
        KNNHeuristic();
    else if(input == 5)
        ACOOnClustersHeuristic();
    else if(input == 6)
        GAHeuristic();
    else if(input == 7)
        ACOHeuristic();
    else if(input == 8)
        ACOCSHeuristic();
    else if(input == 9)
        MMACOHeuristic();
    else if(input == 10)
        CACOHeuristic();

}

/*
 * Calculates the time the heuristic executed for.
 */
double end_heuristic(){
    ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count() - ms;

    for (int i = 0; i < best_sol->steps; ++i)
        printf("%d, ",best_sol->tour[i]);
    printf("\n");

//    printf("time %ld milliseconds\n",ms);
    printf("time %f seconds\n",((double)ms/(double)1000));
    printf("\n");
    return ((double)ms/(double)1000);

    //Clustering.
    //Clusterer::freeClusters();
//    KMeansClustering::freeClusters();
}

/*
 * free memory used by the tour array.
 */
void free_heuristic() {

    delete[] best_sol->tour;
    delete best_sol;
}

