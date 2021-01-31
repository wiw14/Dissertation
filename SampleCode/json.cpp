//
// Created by wmw13 on 31/01/2021.
//

#include <string>
#include "json.hpp"
#include "EVRP.hpp"
FILE* jsonNodes;

void storeNodes(node * nodes){
    if ((jsonNodes = fopen("jsonNodes.json","a")) == NULL) { exit(2);}
    for (int index=0;index<ACTUAL_PROBLEM_SIZE;index++){
        fprintf(jsonNodes,"");
    }
}