#include<cmath>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<math.h>
#include<fstream>
#include<limits.h>

#include "heuristic.hpp"
#include "EVRP.hpp"

/*
 * Approaches.
 */
#include "greedyEuclideanHeuristic.h"
#include "randomHeuristic.h"
#include "ACOHeuristic.h"
#include "DijkstrasHeuristic.h"

using namespace std;

FILE * jsonTour;
solution *best_sol;   //see heuristic.hpp for the solution structure

/*initialize the structure of your heuristic in this function*/
void initialize_heuristic(){

    best_sol = new solution;
    best_sol->tour = new int[NUM_OF_CUSTOMERS+1000];
    best_sol->id = 1;
    best_sol->steps = 0;
    best_sol->tour_length = INT_MAX;
}

void openTourFile(void){
    if ((jsonTour = fopen(R"(C:\Users\wmw13\Documents\GitHub\Dissertation\SampleCode\storeTour.txt)","w")) == NULL) { printf("ERROR\n");}
}
void closeTourFile(void){
    fclose(jsonTour);
}

void storeTour(int run){
    int index = 0;
    while (best_sol->tour[index] != 0 || best_sol->tour[index+1] !=0 || best_sol->tour[index+2] != 0|| best_sol->tour[index+3] !=0 || best_sol->tour[index+4] != 0){
        fprintf(jsonTour,"%d\n",best_sol->tour[index]);
        index ++;
    }
    fprintf(jsonTour,"- %d\n",run);

}

/*implement your heuristic in this function*/
void run_heuristic(){
   //greedyHeuristic();
   //randomHeuristic();
   ACOHeuristic();
   //DijkstrasHeuristic();
}

/*free memory structures*/
void free_heuristic(){
    delete[] best_sol->tour;


}

