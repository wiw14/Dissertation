#include "KNNHeuristic.h"
#include "KNN.h"

/*
 * Creates KNN object which calculates an optimal route based on numOfClusters nearest neighbours.
 */
void KNNHeuristic(int Knn, int RandomSearchIteration, int TwoOptIterations){
    //number of nearest neighbours is passed into the constructor.
    KNN* knn = new KNN(Knn,RandomSearchIteration,TwoOptIterations);
}