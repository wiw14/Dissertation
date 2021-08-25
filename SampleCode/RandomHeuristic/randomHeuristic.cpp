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
void randomHeuristic(int randomSearchIteration,int twoOptIteration){
    auto* LS = new localSearch(randomSearchIteration,twoOptIteration);

    //generate a random solution for the random heuristic
    int i,tempCustomer, offset, tot_assigned =0;
    int *customers;

    customers = new int[NUM_OF_CUSTOMERS+1];
    //set indexes of objects.
    for(i = 0; i <= NUM_OF_CUSTOMERS; i++){
        customers[i]=i;
    }

    //DEBUGGING - prints customer routes before random shift.
//    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++){
//        printf("%d,",customers[index]);
//    }
//    printf("\n");

    //randomly change indexes of objects.
    for(i = 0; i <= NUM_OF_CUSTOMERS; i++){
        offset = (int) ((rand()/(RAND_MAX+1.0)) * (double)((NUM_OF_CUSTOMERS+1)-tot_assigned));
        tempCustomer = customers[i];
        customers[i]=customers[offset+i];
        customers[offset+i]=tempCustomer;
        tot_assigned++;
    }

    //DEBUGGING - prints customer routes after random shift.
//    for (int index = 0; index <= NUM_OF_CUSTOMERS; index++){
//        printf("%d,",customers[index]);
//    }
//    printf("\n");

    //Runs local search on the randomly generated tour.
    LS->randomPheromoneLocalSearchWithTwoOpt(customers);

    //Calculates the quality of the tour created.
    double val = GenerateTour::getRouteLength(customers);


    //de-allocates memory
    delete[] customers;
    delete LS;
}