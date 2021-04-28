#include "CrossoverOperators.h"

/*
 * ================================================================================ *
 * CROSSOVER OPERATOR.
 * ================================================================================ *
 */
/*
 * ================================================================================ *
 * Partially Mapped Crossover Operator.
 * ================================================================================ *
 */
/*
 * checks whether a customer is within a sub-route.
 * used to determine whether mapping is required.
 */
int CrossoverOperators::checkInParition(std::vector<std::pair<int,int>>* subRoutes, int customer, int type, bool* partitionCheck){
    for (int i = 0; i < subRoutes->size(); ++i) {
        if(type == 0 && subRoutes->at(i).first == customer){
            if(partitionCheck[subRoutes->at(i).second])
                return checkInParition(subRoutes,subRoutes->at(i).second,type,partitionCheck);
            else
                return subRoutes->at(i).second;
        }
        else if(type == 1 && subRoutes->at(i).second == customer){
            if(partitionCheck[subRoutes->at(i).first])
                return checkInParition(subRoutes,subRoutes->at(i).first,type,partitionCheck);
            else
                return subRoutes->at(i).first;
        }
    }
    return -1;
}

/*
 * Generates two children by partially mapping a sector of two parents.
 */
int** CrossoverOperators::partiallyMappedCrossover(int* currentBest, int* toCombine){
    /*
     * STEP 1:
     * Find partition mapping points.
     */
    int P1 = rand()%(NUM_OF_CUSTOMERS-1), P2 = rand()%(NUM_OF_CUSTOMERS-P1)+(P1+1);

    /*
     * STEP 2:
     * Generate sub-route.
     */
    auto subRoutes = new std::vector<std::pair<int,int>>();
    for (int i = P1; i <= P2; ++i) {
        auto subRoute = std::pair<int,int>();
        subRoute.first = currentBest[i];
        subRoute.second = toCombine[i];
        subRoutes->push_back(subRoute);
    }

    /*
     * STEP 3:
     * Generate children.
     */
    int *childOne = new int[NUM_OF_CUSTOMERS + 1], *childTwo = new int[NUM_OF_CUSTOMERS + 1];
    bool* partitionCheckCurrentBest = new bool[NUM_OF_CUSTOMERS + 1], * partitionChecktoCombine = new bool[NUM_OF_CUSTOMERS + 1];
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        if(i >= P1 && i <= P2) {
            partitionCheckCurrentBest[currentBest[i]] = true;
            partitionChecktoCombine[toCombine[i]] = true;
        }
        else{
            partitionCheckCurrentBest[currentBest[i]] = false;
            partitionChecktoCombine[toCombine[i]] = false;
        }
    }

    //Create first sector.
    for (int i = 0; i < P1; ++i) {
        if(partitionChecktoCombine[currentBest[i]]){
            //type 1 is toCombine.
            int newCustomer = checkInParition(subRoutes,currentBest[i],1,partitionChecktoCombine);
            childOne[i] = newCustomer;
        }
        else{
            childOne[i] = currentBest[i];
        }

        if(partitionCheckCurrentBest[toCombine[i]]){
            //type 0 is currentBest.
            int newCustomer = checkInParition(subRoutes,toCombine[i],0,partitionCheckCurrentBest);
            childTwo[i] = newCustomer;
        }
        else{
            childTwo[i] = toCombine[i];
        }
    }

    for (int i = P1; i <= P2 ; ++i) {
        childOne[i] = toCombine[i];
        childTwo[i] = currentBest[i];
    }

    //Create second sector.
    for (int i = P2+1; i <= NUM_OF_CUSTOMERS; ++i) {
        if(partitionChecktoCombine[currentBest[i]]){
            //type 1 is toCombine.
            int newCustomer = checkInParition(subRoutes,currentBest[i],1,partitionChecktoCombine);
            childOne[i] = newCustomer;
        }
        else{
            childOne[i] = currentBest[i];
        }

        if(partitionCheckCurrentBest[toCombine[i]]){
            //type 1 is toCombine.
            int newCustomer = checkInParition(subRoutes,toCombine[i],0,partitionCheckCurrentBest);
            childTwo[i] = newCustomer;
        }
        else{
            childTwo[i] = toCombine[i];
        }
    }
    delete[] partitionCheckCurrentBest;
    delete[] partitionChecktoCombine;
    delete subRoutes;

    int** childPopulation = new int*[2];
    childPopulation[0] = childOne; childPopulation[1] = childTwo;
    return childPopulation;
}


/*
 * ================================================================================ *
 * Initial Recombination Operator (Requires Repairing).
 * ================================================================================ *
 */
/*
 * Alternates between two parents to create a child based upon whether parent1[i] == parent2[i].
 */
int** CrossoverOperators::testRecombination(int *currentBest, int *toCombine) {
    int *childOne = new int[NUM_OF_CUSTOMERS + 1], *childTwo = new int[NUM_OF_CUSTOMERS + 1];
    bool *visitedOne = new bool[NUM_OF_CUSTOMERS + 1], *visitedTwo = new bool[NUM_OF_CUSTOMERS + 1];

    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        visitedOne[i] = false;
        visitedTwo[i] = false;
    }

    bool SWITCH = false;
    //Loops through all customers.
    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index) {
        if (currentBest[index] == toCombine[index]) {
            SWITCH = !SWITCH;
            //Loops through all the customers which are the same in both parents.
            while (currentBest[index] == toCombine[index] && index <= NUM_OF_CUSTOMERS) {
                childOne[index] = currentBest[index];
                visitedOne[childOne[index]] = true;
                childTwo[index] = currentBest[index];
                visitedTwo[childTwo[index]] = true;
                index++;
            }
            index--;
        }
        //Parent 1 [i] != Parent 2 [i].
        else {
            //Child 1 [i] = Parent 1 [i].
            //Child 2 [i] = Parent 2 [i].
            if (SWITCH && !visitedOne[currentBest[index]] && !visitedTwo[toCombine[index]]) {
                childOne[index] = currentBest[index];
                visitedOne[childOne[index]] = true;
                childTwo[index] = toCombine[index];
                visitedTwo[childTwo[index]] = true;
            }else {
                //Child 1 [i] = Parent 2 [i].
                //Child 2 [i] = Parent 1 [i].
                if (!visitedOne[toCombine[index]] && !visitedTwo[currentBest[index]]) {
                    childOne[index] = toCombine[index];
                    visitedOne[childOne[index]] = true;
                    childTwo[index] = currentBest[index];
                    visitedTwo[childTwo[index]] = true;
                }
                //Child 1 [i] = Parent 1 [i].
                //Child 2 [i] = Parent 2 [i].
                else {
                    childOne[index] = currentBest[index];
                    visitedOne[childOne[index]] = true;
                    childTwo[index] = toCombine[index];
                    visitedTwo[childTwo[index]] = true;
                }
            }
        }
    }
    //Frees used memory.
    delete[] visitedOne;
    delete[] visitedTwo;

    //Creates array to return the children to the Genetic Algorithm.
    int ** children = new int*[2];
    children[0] = childOne; children[1] = childTwo;
    return children;
}

/*
 * ================================================================================ *
 * Partition Crossover Operator.
 * ================================================================================ *
 */

/*
 * Generates a table of edges based on the two parents.
 */
std::map<int, std::pair<int, std::string> *>* CrossoverOperators::createEdgeTable(int* currentBest, int* toCombine){
    /*
     * STEP 1a:
     * Creates the edge table and instantiates the table.
     */
    auto edgeTable = new std::map<int, std::pair<int, std::string> *>();
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        auto *tempArray = new std::pair<int, std::string>[4];
        for (int j = 0; j < 4; ++j) {
            tempArray[j].first = INT_MAX;
            tempArray[j].second = "";
        }
        edgeTable->emplace(i, tempArray);
    }

    /*
     * STEP 1b:
     * Fills the edge table
     */
    //Lambda function for adding the edge to the table.
    auto addToArray = [&](std::pair<int, std::string> *array, const int *currentArray, int index) {
        for (int arrayIndex = 0; arrayIndex < 4; ++arrayIndex) {
            if (array[arrayIndex].first == INT_MAX) {
                array[arrayIndex].first = currentArray[index];
                break;
            } else if (array[arrayIndex].first == currentArray[index]) {
                array[arrayIndex].second = "'";
                break;
            }
        }
    };

    //Loop through all the customers in both routes and generate an edge table.
    for (int customerIndex = 0; customerIndex <= NUM_OF_CUSTOMERS; ++customerIndex) {
        std::pair<int, std::string> *bestArray = edgeTable->at(currentBest[customerIndex]);
        std::pair<int, std::string> *toCombineArray = edgeTable->at(toCombine[customerIndex]);

        if (customerIndex != 0) {
            addToArray(bestArray, currentBest, customerIndex - 1);
            addToArray(toCombineArray, toCombine, customerIndex - 1);
            if (customerIndex == NUM_OF_CUSTOMERS) {
                addToArray(bestArray, currentBest, 0);
                addToArray(toCombineArray, toCombine, 0);
            }
        }
        if (customerIndex != NUM_OF_CUSTOMERS) {
            addToArray(bestArray, currentBest, customerIndex + 1);
            addToArray(toCombineArray, toCombine, customerIndex + 1);
            if (customerIndex == 0) {
                addToArray(bestArray, currentBest, NUM_OF_CUSTOMERS);
                addToArray(toCombineArray, toCombine, NUM_OF_CUSTOMERS);
            }
        }
    }

    //Return edge table
    return edgeTable;
}

/*
 * Displays the edge table generated by the two parents.
 */
void CrossoverOperators::displayEdgeTable(std::map<int, std::pair<int, std::string> *>* edgeTable){
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        printf("%d : ", i);
        std::pair<int, std::string> *tempArray = edgeTable->at(i);
        for (int j = 0; j < 4; ++j) {
            if (tempArray[j].first == INT_MAX)
                break;
            printf("%d%s, ", tempArray[j].first, tempArray[j].second.c_str());
        }
        printf("\n");
    }
    printf("-----------------------------\n");
}

/*
 * Creates a list of customers which have greater than 2 edges.
 * These customers have different edges connecting to them in the two parents.
 */
std::list<int>* CrossoverOperators::createDegreeList(std::map<int, std::pair<int, std::string> *>* edgeTable){
    auto degreeList = new std::list<int>();

    int count;
    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
        count = 0;
        std::pair<int, std::string> * tempArray = edgeTable->at(i);
        for (int j = 0; j < 4; ++j) {
            if(tempArray[j].first == INT_MAX)
                break;
            count++;
        }
        if(count > 2)
            degreeList->push_back(i);
    }

    return degreeList;
}

/*
 * Checks whether the customer has already been visited.
 * This is done by checking whether the customer is present in the list of customers with more than 2 edges.
 */
bool CrossoverOperators::checkIfCustomerVisited(std::list<int>* degreeList, int customer){
    bool visited = true;
    for (auto i : *degreeList){
        if(i == customer){
            visited = false;
            break;
        }
    }
    return visited;
}

/*
 * Displays a std::list by iterating over it.
 * Used for debugging degreeList.
 */
void CrossoverOperators::displayList(std::list<int>* list){
    for (auto i : *list)
        printf("%d, ", i);
    printf("\n");
}

/*
 * Generates partitions within the two parent routes based upon breadth first search through the unique edges present in
 * the edge table.
 */
std::vector<std::list<int>*>* CrossoverOperators::createPartitions(std::map<int, std::pair<int, std::string> *>* edgeTable, std::list<int>* degreeList){
    auto partitions = new std::vector<std::list<int>*>();
    auto queue = new std::deque<int>();
//    displayList(degreeList);
    while(!degreeList->empty()) {
        queue->push_back(degreeList->front());
        auto partition = new std::list<int>();
        partition->push_back(degreeList->front());
        degreeList->pop_front();

        while (!queue->empty()) {
            int currentNode = queue->front();
            queue->pop_front();
            std::pair<int, std::string> *tempArray = edgeTable->at(currentNode);
            for (int i = 0; i < 4; ++i) {
                if(tempArray[i].first == INT_MAX)
                    break;
                if(tempArray[i].second == "'")
                    continue;
                else if(!checkIfCustomerVisited(degreeList,tempArray[i].first)){
                    degreeList->remove(tempArray[i].first);
                    queue->push_back(tempArray[i].first);
                    partition->push_back(tempArray[i].first);
//                    displayList(degreeList);
                }
            }
            delete[] tempArray;
        }
        partitions->push_back(partition);
    }

    delete queue;
    return partitions;
}

/*
 * Returns the partition the customer is present in.
 */
int isInPartition(std::list<int>* partitionOne, std::list<int>* partitionTwo, int customer){
    auto positionOne = std::find(partitionOne->begin(),partitionOne->end(),customer);
    if (positionOne != partitionOne->end()){
        // Found in partition one.
        return 1;
    }
    else{
        auto positionTwo = std::find(partitionTwo->begin(),partitionTwo->end(),customer);
        if(positionTwo != partitionTwo->end()) {
            //Found in partition two.
            return 2;
        }
    }
    //Not in either partition.
    return 0;
}

/*
 * Partition Crossover Operator, generates children based on alternating partitions located within parent routes.
 * Version 2.0
 */
int** CrossoverOperators::PCRecombine(int * currentBest, int * toCombine) {
    auto LS = new localSearch(3,3);

    /*
     * STEP 1:
     * Create edge table.
     */
    auto edgeTable = createEdgeTable(currentBest,toCombine);
//    displayEdgeTable(edgeTable);

    /*
     * STEP 2:
     * Create list without 2 degree vertices.
     */
    auto degreeList = createDegreeList(edgeTable);

    /*
     * STEP 3:
     * Create partitions.
     */
    auto partitionList = createPartitions(edgeTable,degreeList);

    int smallParitionSize = INT_MAX, smallestPartition = -1,counter = 1;
    for (auto p : *partitionList) {
        int paritionSize = 0;
        for (auto c : *p) {
            paritionSize++;
        }
        if(paritionSize < INT_MAX){
            smallParitionSize = paritionSize;
            smallestPartition = counter-1;
        }
        counter++;
    }

    /*
     * STEP 4:
     * Generate children from partitions.
     */
    int** tempChildren = new int*[2];

    if(partitionList->size() > 1){
        printf("S, ");
        auto partitionA = new std::list<int>();
        auto partitionB = new std::list<int>();

        for (int customerIndex = 0; customerIndex <=  NUM_OF_CUSTOMERS; ++customerIndex) {
            for (auto p : *partitionList->at(smallestPartition)) {
                if(currentBest[customerIndex] == p){
                    partitionA->push_back(currentBest[customerIndex]);
                }
                if(toCombine[customerIndex] == p){
                    partitionB->push_back(toCombine[customerIndex]);
                }
            }
        }

        int *childOne = new int[NUM_OF_CUSTOMERS + 1], *childTwo = new int[NUM_OF_CUSTOMERS + 1];

        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
            bool check = false;
            for (auto p : *partitionList->at(smallestPartition)) {
                if(currentBest[i] == p){
                    childOne[i] = partitionB->front();
                    partitionB->pop_front();
                    check = true;
                    break;
                }
            }
            if(!check){
                childOne[i] = currentBest[i];
            }
        }

        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
            bool check = false;
            for (auto p : *partitionList->at(smallestPartition)) {
                if(toCombine[i] == p){
                    childTwo[i] = partitionA->front();
                    partitionA->pop_front();
                    check = true;
                    break;
                }
            }
            if(!check){
                childTwo[i] = toCombine[i];
            }
        }

        tempChildren[0] = childOne; tempChildren[1] = childTwo;
    }
    else{
        printf("F, ");

        int *child = new int[NUM_OF_CUSTOMERS + 1];
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
            child[i] = toCombine[i];

        LS->randomPheromoneLocalSearchWithTwoOpt(child);

        tempChildren[0] = child;
        int* fakeChild = new int[NUM_OF_CUSTOMERS+1];
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
            fakeChild[i] = INT_MAX;
        tempChildren[1] = fakeChild;
    }
    delete LS;
    delete edgeTable;
    delete degreeList;
    for (auto p : *partitionList) {
        delete p;
    }
    delete partitionList;

    return tempChildren;
}

/*
 * ================================================================================ *
 * Generalised Partition Crossover Operator.
 * ================================================================================ *
 */
/*
 * Locates which partition the customer is in.
 */
int CrossoverOperators::findInPartiton(std::vector<std::list<int>*>* partitions, int customer){
    int pCounter = 0;
    for (auto p : *partitions) {
        pCounter++;
        for (auto c : *p) {
            if(c == customer)
                return pCounter;
        }
    }
    return -1;
}

/*
 * Generalised Partition Crossover Operator, generates children based on alternating partitions located within parent routes.
 * Version 1.0 (Does not produce optimal results)
 */
int** CrossoverOperators::GPCRecombine(int * currentBest, int * toCombine) {
    auto LS = new localSearch(3,3);

    /*
     * STEP 1:
     * Create edge table.
     */
    auto edgeTable = createEdgeTable(currentBest,toCombine);
//    displayEdgeTable(edgeTable);

    /*
     * STEP 2:
     * Create list without 2 degree vertices.
     */
    auto degreeList = createDegreeList(edgeTable);

    /*
     * STEP 3:
     * Create partitions.
     */
    auto partitionList = createPartitions(edgeTable,degreeList);

    /*
     * STEP 4:
     * Generate children from partitions.
     */
    int** tempChildren = new int*[2];
    if(partitionList->size() > 1){
        int *childOne = new int[NUM_OF_CUSTOMERS + 1], *childTwo = new int[NUM_OF_CUSTOMERS + 1];
        bool* visited = new bool[NUM_OF_CUSTOMERS+1];
        for (int customerCounter = 0; customerCounter <= NUM_OF_CUSTOMERS; ++customerCounter)
            visited[customerCounter] = false;
        for (int customerCounter = 0; customerCounter <= NUM_OF_CUSTOMERS; ++customerCounter) {
            int type = findInPartiton(partitionList, currentBest[customerCounter]);
            if ((type == -1 || type % 2 == 0) && !visited[currentBest[customerCounter]]) {
            visited[currentBest[customerCounter]] = true;
            childOne[customerCounter] = currentBest[customerCounter];
        }
            else if(!visited[toCombine[customerCounter]]) {
                visited[toCombine[customerCounter]] = true;
                childOne[customerCounter] = toCombine[customerCounter];
            }
            else{
                visited[customerCounter] = true;
                childOne[customerCounter] = customerCounter;
            }

        }
        for (int customerCounter = 0; customerCounter <= NUM_OF_CUSTOMERS; ++customerCounter)
            visited[customerCounter] = false;
        for (int customerCounter = 0; customerCounter <= NUM_OF_CUSTOMERS; ++customerCounter) {
            int type = findInPartiton(partitionList,toCombine[customerCounter]);
            if((type == -1 || type%2 == 0 ) && !visited[toCombine[customerCounter]]) {
                visited[toCombine[customerCounter]] = true;
                childTwo[customerCounter] = toCombine[customerCounter];
            }
            else if(!visited[currentBest[customerCounter]]) {
                visited[currentBest[customerCounter]] = true;
                childTwo[customerCounter] = currentBest[customerCounter];
            }
            else {
                visited[customerCounter] = true;
                childTwo[customerCounter] = customerCounter;
            }

        }
        delete[] visited;
        tempChildren[0] = childOne; tempChildren[1] = childTwo;
    }
    else{
        printf("F, ");

        int *child = new int[NUM_OF_CUSTOMERS + 1];
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
            child[i] = toCombine[i];

        LS->randomPheromoneLocalSearchWithTwoOpt(child);
        tempChildren[0] = child;
        int* fakeChild = new int[NUM_OF_CUSTOMERS+1];
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
            fakeChild[i] = INT_MAX;
        tempChildren[1] = fakeChild;
    }
    delete LS;
    delete edgeTable;
    delete degreeList;
    for (auto p : *partitionList) {
        delete p;
    }
    delete partitionList;

    return tempChildren;
}

/*
 * ================================================================================ *
 * Old Code
 * ================================================================================ *
 */

/*
 * Partition Crossover Operator, generates children based on alternating partitions located within parent routes.
 * Version 1.0
 */
//int** CrossoverOperators::PCRecombine(int *currentBest, int *toCombine) {
//    /*
//     * Recombination of routes,
//     * add recombinations to child population.
//     */
//    auto edgeTable = new std::map<int, std::pair<int, std::string> *>();
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
//        auto *tempArray = new std::pair<int, std::string>[4];
//        for (int j = 0; j < 4; ++j) {
//            tempArray[j].first = INT_MAX;
//            tempArray[j].second = "";
//        }
//        edgeTable->emplace(i, tempArray);
//    }
//
//    //DEBUGGING
//    printf("Current Best : ");
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//        printf("%d, ", currentBest[i]);
//    printf("\n");
//    printf("To Combine : ");
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//        printf("%d, ", toCombine[i]);
//    printf("\n");
//    /*
//     * STEP 1:
//     * Create edge table.
//     */
//    //Lambda function for adding vertices to edge array.
//    auto addToArray = [&](std::pair<int, std::string> *array, const int *currentArray, int index) {
//
//        for (int arrayIndex = 0; arrayIndex < 4; ++arrayIndex) {
//            if (array[arrayIndex].first == INT_MAX) {
//                array[arrayIndex].first = currentArray[index];
//                break;
//            } else if (array[arrayIndex].first == currentArray[index]) {
//                array[arrayIndex].second = "'";
//                break;
//            }
//        }
//    };
//
//    //Loop through all the customers in both routes and generate an edge table.
//    for (int customerIndex = 0; customerIndex <= NUM_OF_CUSTOMERS; ++customerIndex) {
//        std::pair<int, std::string> *bestArray = edgeTable->at(currentBest[customerIndex]);
//        std::pair<int, std::string> *toCombineArray = edgeTable->at(toCombine[customerIndex]);
//
//        if (customerIndex != 0) {
//            addToArray(bestArray, currentBest, customerIndex - 1);
//            addToArray(toCombineArray, toCombine, customerIndex - 1);
//            if (customerIndex == NUM_OF_CUSTOMERS) {
//                addToArray(bestArray, currentBest, 0);
//                addToArray(toCombineArray, toCombine, 0);
//            }
//        }
//        if (customerIndex != NUM_OF_CUSTOMERS) {
//            addToArray(bestArray, currentBest, customerIndex + 1);
//            addToArray(toCombineArray, toCombine, customerIndex + 1);
//            if (customerIndex == 0) {
//                addToArray(bestArray, currentBest, NUM_OF_CUSTOMERS);
//                addToArray(toCombineArray, toCombine, NUM_OF_CUSTOMERS);
//            }
//        }
//    }
//
//    //Displays edge table.
//    //DEBUGGING
////    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
////        printf("%d : ", i);
////        std::pair<int, std::string> *tempArray = edgeTable->at(i);
////        for (int j = 0; j < 4; ++j) {
////            if (tempArray[j].first == INT_MAX)
////                break;
////            printf("%d%s, ", tempArray[j].first, tempArray[j].second.c_str());
////        }
////        printf("\n");
////    }
////    printf("-----------------------------\n");
//
//    /*
//    * STEP 2a:
//     * Creates additional lists.
//    */
//
//    //Creates and initialises LIST1.
//    int numDegreeTwo = 0;
//    auto LIST1 = new std::list<int>(); //insert at both head and tail.
//    for (int customerIndex = 0; customerIndex <= NUM_OF_CUSTOMERS; ++customerIndex) {
//        int degree = 0;
//        for (int arrayIndex = 0; arrayIndex < 4; ++arrayIndex) {
//            if (edgeTable->at(customerIndex)[arrayIndex].first == INT_MAX)
//                break;
//            degree++;
//        }
////        printf("%d=%d\n", customerIndex, degree);
//        if (degree > 2) {
//            LIST1->push_back(customerIndex);
//        } else {
//            LIST1->push_front(customerIndex);
//            numDegreeTwo++;
//        }
//    }
//    int cPointer = numDegreeTwo;
//
//    //Creates and initialises LIST2, index list.
//    auto indexList = new int[NUM_OF_CUSTOMERS + 1];
//    //Indexing.
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
//        indexList[*std::next(LIST1->begin(), i)] = i;
//    }
//    int iPointer = NUM_OF_CUSTOMERS;
//
//    //DEBUGGING
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//        printf("%d:%d,", i, indexList[i]);
//    printf("\n");
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//        printf("%d:%d,", i, *std::next(LIST1->begin(), i));
//    printf("\n");
//    printf("%d:%d, %d:%d\n", iPointer, *std::next(LIST1->begin(), iPointer), cPointer,
//           *std::next(LIST1->begin(), cPointer));
//
//    /*
//     * STEP 2b:
//     * Generate partitions.
//     */
//    auto customerQueue = std::deque<int>();
//    auto partitioned = new bool[NUM_OF_CUSTOMERS + 1];
//    int partitionCounter = 0;
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//        partitioned[i] = false;
//    while (!(iPointer == numDegreeTwo)) {
//        cPointer = numDegreeTwo;
//        customerQueue.push_front(*std::next(LIST1->begin(), cPointer));
//        printf("%d\n", partitionCounter);
//        printf("==%d:%d, %d:%d\n", iPointer, *std::next(LIST1->begin(), iPointer), cPointer,
//               *std::next(LIST1->begin(), cPointer));
//        partitionCounter++;
//        while (!customerQueue.empty()) {
//            int currentNode = customerQueue.back();
//            partitioned[currentNode] = true;
//            customerQueue.pop_back();
//            cPointer = indexList[currentNode];
//            printf("Current Node = %d\n", currentNode);
//            if (iPointer < cPointer || iPointer == numDegreeTwo)
//                break;
//            for (int i = 0; i < 4; ++i) {
//                if (edgeTable->at(currentNode)[i].first == INT_MAX)
//                    break;
//                else if (edgeTable->at(currentNode)[i].second != "'") {
//                    edgeTable->at(currentNode)[i].second = std::to_string(partitionCounter);
//                    if (!partitioned[edgeTable->at(currentNode)[i].first])
//                        customerQueue.push_front(edgeTable->at(currentNode)[i].first);
//                }
//            }
//            indexList[*std::next(LIST1->begin(), cPointer)] = iPointer;
//            indexList[*std::next(LIST1->begin(), iPointer)] = cPointer;
//
//            std::swap(*std::next(LIST1->begin(), cPointer), *std::next(LIST1->begin(), iPointer));
//            printf("%d:%d, %d:%d\n", iPointer, *std::next(LIST1->begin(), iPointer), cPointer,
//                   *std::next(LIST1->begin(), cPointer));
//
//            for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//                printf("%d:%d,", i, *std::next(LIST1->begin(), i));
//            printf("\n");
//            iPointer--;
//        }
//    }
//
//    //DEBUGGING
//    printf("NUMBER OF PARTITIONS : %d\n", partitionCounter);
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
//        printf("%d : ", i);
//        std::pair<int, std::string> *tempArray = edgeTable->at(i);
//        for (int j = 0; j < 4; ++j) {
//            if (tempArray[j].first == INT_MAX)
//                break;
//            printf("%d:%s, ", tempArray[j].first, tempArray[j].second.c_str());
//        }
//        printf("\n");
//    }
//    printf("-----------------------------\n");
//
//    /*
//     * STEP 3
//     * Create children from partitions.
//     */
//    if (partitionCounter > 1) {
//        //DEBUGGING
//        printf("PX Success.\n");
//        int *childOne = new int[NUM_OF_CUSTOMERS + 1], *childTwo = new int[NUM_OF_CUSTOMERS + 1];
//        int partitionA = -1, partitionB = -1;
//        if (partitionCounter == 2) {
//            partitionA = 1;
//            partitionB = 2;
//        } else {
//            //Find 2 largest partition
//            int *partitionSizes = new int[2]{-1, -1};
//            for (int partitionIndex = 1; partitionIndex <= partitionCounter; ++partitionIndex) {
//                int partitionSize = 0;
//                for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
//                    std::string temp = "";
//                    for (int j = 0; j < 4; ++j) {
//                        if (edgeTable->at(i)[j].first == INT_MAX)
//                            break;
//                        else if (edgeTable->at(i)[j].second != "'") {
//                            temp = edgeTable->at(i)[j].second;
//                            break;
//                        }
//                    }
//                    if (temp == std::to_string(partitionCounter)) {
//                        partitionSize++;
//                    }
//                }
//                if (partitionSizes[0] < partitionSize) {
//                    partitionSizes[1] = partitionSizes[0];
//                    partitionSizes[0] = partitionSize;
//                    partitionB = partitionA;
//                    partitionA = partitionIndex;
//                } else if (partitionSizes[1] < partitionSize) {
//                    partitionSizes[1] = partitionSize;
//                    partitionB = partitionIndex;
//                }
//            }
//            delete[] partitionSizes;
//        }
//        printf("A: %d, B: %d\n", partitionA, partitionB);
//
//        std::string type = "";
//        for (int i = 0; i <= NUM_OF_CUSTOMERS ; ++i) {
//            std::pair<int, std::string> *edges = edgeTable->at(currentBest[i]);
//
//
//            for (int j = 0; j < 4; ++j) {
//                if(edges[j].first == INT_MAX)
//                    break;
//                if(edges[j].second != "'" && edges[j].second != ""){
//                    type = edges[j].second;
//                 break;}
//            }
//            if (type == std::to_string(partitionB))
//                childOne[i] = toCombine[i];
//            else
//                childOne[i] = currentBest[i];
//        }
//        type = "";
//        for (int i = 0; i <= NUM_OF_CUSTOMERS ; ++i) {
//            std::pair<int, std::string> *edges = edgeTable->at(toCombine[i]);
//
//
//            for (int j = 0; j < 4; ++j) {
//                if(edges[j].first == INT_MAX)
//                    break;
//                if(edges[j].second != "'" && edges[j].second != ""){
//                    type = edges[j].second;
//                    break;}
//            }
//            if (type == std::to_string(partitionB))
//                childTwo[i] = currentBest[i];
//            else
//                childTwo[i] = toCombine[i];
//        }
//
//        printf("\n");
//        printf("child one: ");
//        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//            printf("%d, ",childOne[i]);
//        printf("\n");
//        printf("child two: ");
//        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//            printf("%d, ",childTwo[i]);
//        printf("\n");
//
//        childPopulation[childPopulationCounter++] = childOne;
//        childPopulation[childPopulationCounter++] = childTwo;
//    } else {
//        //DEBUGGING
//        printf("PX Failure.\n");
//        int *child = new int[NUM_OF_CUSTOMERS + 1];
//        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//            child[i] = toCombine[i];
//        //printf("-%d\n",childPopulationCounter);
//        LS->randomPheromoneLocalSearchWithTwoOpt(child);
//        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//            printf("%d, ",child[i]);
//        printf("\n");
//        childPopulation[childPopulationCounter++] = child;
//    }
//
//    /*
//     * STEP 4:
//     * Delete the edge table from memory.
//     */
//    for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i)
//        delete[] edgeTable->at(i);
//    delete edgeTable;
//}