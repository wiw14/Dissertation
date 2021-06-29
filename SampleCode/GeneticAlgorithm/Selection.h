//
// Created by wmw13 on 05/03/2021.
//

#ifndef TESTSAMPLECODE_SELECTION_H
#define TESTSAMPLECODE_SELECTION_H

#include "RepairOperators.h"
#include <algorithm>

class Selection {
public:
    static int** greedySelection(int **, int, int);
    static int** correlativeFamilyBasedSelection(int **, int, int);
    static int** firstHalf(int **, int, int);
    static int** generateHammingDistanceArray(int**,int, int**,int);
    static int calculateHammingDistance(const int*,const int*);
};
#endif //TESTSAMPLECODE_SELECTION_H
