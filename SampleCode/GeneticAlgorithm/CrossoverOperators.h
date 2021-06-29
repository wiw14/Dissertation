#ifndef TESTSAMPLECODE_CROSSOVEROPERATORS_H
#define TESTSAMPLECODE_CROSSOVEROPERATORS_H

#include <list>
#include <deque>
#include "../GeneticAlgorithm/Mutation.h"

class CrossoverOperators {
private:
    static int checkInParition(std::vector<std::pair<int,int>>*, int, int, bool*);
    static std::map<int, std::pair<int, std::string> *>* createEdgeTable(int*,int*);
    static void displayEdgeTable(std::map<int, std::pair<int, std::string> *>*);
    static std::list<int>* createDegreeList(std::map<int, std::pair<int, std::string> *>*);
    static std::vector<std::list<int>*>* createPartitions(std::map<int, std::pair<int, std::string> *>*, std::list<int>* );
    static bool checkIfCustomerVisited(std::list<int>*, int);
    static void displayList(std::list<int>*);
    static int findInPartiton(std::vector<std::list<int>*>*, int);
public:
    static int** testRecombination(int*, int*);
    static int** partiallyMappedCrossover(int*, int*);
    static int** PCRecombine(int*, int*);
    static int** GPCRecombine(int*, int*);
};


#endif //TESTSAMPLECODE_CROSSOVEROPERATORS_H
