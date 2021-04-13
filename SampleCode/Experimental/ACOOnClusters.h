//
// Created by wmw13 on 09/03/2021.
//

#ifndef TESTSAMPLECODE_ACOONCLUSTERS_H
#define TESTSAMPLECODE_ACOONCLUSTERS_H
#include "ClusterACO.h"
void ACOOnClusters(int);
int* generateCostTable(int, std::vector<int*>,int*);
void twoOptForCluster(int* bestRoute, int clusterSize, int twoOptIterations);
void twoOptSwapForClusters(int i, int j, int *route, const int *currRoute,int routeSize);
double getClusterRouteLength(int* route, int clusterSize);
int *generateRouteFromClusters(int sizeOfAlphabet, std::vector<int *> clusters, int *sizes);

#endif //TESTSAMPLECODE_ACOONCLUSTERS_H
