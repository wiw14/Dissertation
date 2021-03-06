//
// Created by wmw13 on 05/03/2021.
//

#include "Mutation.h"

void Mutation::randomSwapMutation(int* route){
    int pos1 = rand()%NUM_OF_CUSTOMERS, pos2 = rand()%(NUM_OF_CUSTOMERS+1-pos1)+pos1;
    int temp = route[pos1];
    route[pos1] = route[pos2];
    route[pos2] = temp;
}

void Mutation::LKMutation(int * route, localSearch* LS) {
    LS->LKSearch(route);
}