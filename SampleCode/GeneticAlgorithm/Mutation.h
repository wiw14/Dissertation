//
// Created by wmw13 on 05/03/2021.
//

#ifndef TESTSAMPLECODE_MUTATION_H
#define TESTSAMPLECODE_MUTATION_H

#include "../GeneticAlgorithm/Selection.h"

class Mutation {
public:
    static void randomSwapMutation(int*);
    static void LKMutation(int*, localSearch*);
};


#endif //TESTSAMPLECODE_MUTATION_H
