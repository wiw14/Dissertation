//
// Created by wmw13 on 06/03/2021.
//

#ifndef TESTSAMPLECODE_CLKSEARCH_H
#define TESTSAMPLECODE_CLKSEARCH_H

#include "../LocalSearchAndGenerateTour/LocalSearches.h"
class CLKSearch {
private:
    int* route;
    void generaterRandomTour();
    void createInitialRoute();
    void displayRoute();
public:
    CLKSearch();
    ~CLKSearch();


};


#endif //TESTSAMPLECODE_CLKSEARCH_H
