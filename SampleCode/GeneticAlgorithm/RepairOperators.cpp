#include "RepairOperators.h"

/*
 * ================================================================================ *
 * REPAIR OPERATORS.
 * ================================================================================ *
 */

void RepairOperators::basicRepair(int *route) {
    bool* visited = new bool[NUM_OF_CUSTOMERS+1];
    auto duplicates = std::list<int>();

    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        if(visited[route[i]])
            duplicates.push_back(i);
        visited[route[i]] = true;
    }

    while(!duplicates.empty()){
        int customerIndex = duplicates.front();
        duplicates.pop_front();

        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
            if(!visited[i]){
                route[customerIndex] = i;
                visited[i] = true;
                break;
            }
        }
    }

    delete[] visited;
}