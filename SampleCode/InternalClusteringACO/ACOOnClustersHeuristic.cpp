#include "ACOOnClustersHeuristic.h"

/*
 * Runs the internal Clustered ACO.
 */
void ACOOnClustersHeuristic(int nAnts, int iter, int probSize, double pheroDec, double q, double al, double be, int rsi, int toi){
    ACOOnClusters(nAnts,iter,probSize,pheroDec,q,al,be,rsi,toi);
}