#include<cmath>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<math.h>
#include<fstream>
#include<time.h>
#include<limits.h>

#include "EVRP.hpp"
#include "stats.hpp"
#include "heuristic.hpp"

using namespace std;

//Used to output offline performance and population diversity

FILE *log_performance;
//output files
char *perf_filename;

FILE * jsonTour;


double mean(double* values, int size){
    int i;
    double m = 0.0;
    for (i = 0; i < size; i++){
        m += values[i];
    }
    m = m / (double)size;
    return m; //mean
}

double stdev(double* values, int size, double average){
    int i;
    double dev = 0.0;

    if (size <= 1)
        return 0.0;
    for (i = 0; i < size; i++){
        dev += ((double)values[i] - average) * ((double)values[i] - average);
    }
    return sqrt(dev / (double)(size - 1)); //standard deviation
}

double best_of_vector(double *values, int l ) {
    double min;
    int k;
    k = 0;
    min = values[k];
    for( k = 1 ; k < l ; k++ ) {
        if( values[k] < min ) {
            min = values[k];
        }
    }
    return min;
}


double worst_of_vector(double *values, int l ) {
    double max;
    int k;
    k = 0;
    max = values[k];
    for( k = 1 ; k < l ; k++ ) {
        if( values[k] > max ){
            max = values[k];
        }
    }
    return max;
}



