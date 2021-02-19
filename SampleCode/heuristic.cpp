#include<cmath>
#include<string>
#include<limits.h>

#include "EVRP.hpp"
#include "greedyEuclideanHeuristic.h"
#include "randomHeuristic.h"
#include "ACOHeuristic.h"
#include "DijkstrasHeuristic.h"
#include "KNNHeuristic.h"
#include "GAHeuristic.h"
#include "heuristic.hpp"

using namespace std;

solution *best_sol;

/*
 * Initialize the structure of your heuristic in this function
 */
void initialize_heuristic() {
    best_sol = new solution;
    best_sol->tour = new int[NUM_OF_CUSTOMERS + 1000];
    best_sol->id = 1;
    best_sol->steps = 0;
    best_sol->tour_length = INT_MAX;
}


/*
 * List of all the currently implemented heuristics.
 * Activate heuristics by uncommenting.
 */
void run_heuristic() {
    //greedyHeuristic();
    //randomHeuristic();
    ACOHeuristic();
    //ACOCSHeuristic();
    //DijkstrasHeuristic();
//    KNNHeuristic();
    //GAHeuristic();
}


/*
 * free memory used by the tour array.
 */
void free_heuristic() {
    delete[] best_sol->tour;


}

