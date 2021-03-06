#include "Mutation.h"

/*
 * Randomly switches two customers within the route to generate new edges.
 */
void Mutation::randomSwapMutation(int* route){
    int pos1 = rand()%NUM_OF_CUSTOMERS, pos2 = rand()%(NUM_OF_CUSTOMERS+1-pos1)+pos1;
    int temp = route[pos1];
    route[pos1] = route[pos2];
    route[pos2] = temp;
}

/*
 * Uses the Lin-Kernighan local search to Mutation the route.
 */
void Mutation::LKMutation(int * route, localSearch* LS) {
    LS->LKSearch(route);
}