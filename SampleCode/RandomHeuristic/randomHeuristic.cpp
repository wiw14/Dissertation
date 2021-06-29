#include<cmath>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<math.h>
#include<fstream>
#include<limits.h>

#include "randomHeuristic.h"
#include "../LocalSearchAndGenerateTour/LocalSearches.h"
/*
 * ================================================================================ *
 * RANDOM NEXT CUSTOMER HEURISTIC
 * ================================================================================ *
 */

/*
 * Creates local search object.
 * Randomly picks a next customer until all customers have been chosen.
 * Evaluates the tour generated.
 */
void randomHeuristic(){
    auto* LS = new localSearch(3,3);

    //generate a random solution for the random heuristic
    int i,help, object, tot_assigned =0;
    int *r;

    r = new int[NUM_OF_CUSTOMERS+1];
    //set indexes of objects.
    for(i = 1; i <= NUM_OF_CUSTOMERS; i++){
        r[i-1]=i;

    }
    //randomly change indexes of objects.
    for(i = 0; i <= NUM_OF_CUSTOMERS; i++){
        object = (int) ((rand()/(RAND_MAX+1.0)) * (double)(NUM_OF_CUSTOMERS-tot_assigned));
        help = r[i];
        r[i]=r[i+object];
        r[i+object]=help;
        tot_assigned++;
    }

    //Runs local search on the randomly generated tour.
    LS->randomPheromoneLocalSearchWithTwoOpt(r);

    //Calculates the quality of the tour created.
    double val = GenerateTour::getRouteLength(r);


    //de-allocates memory
    delete[] r;
    delete LS;
}