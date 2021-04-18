 #include "Selection.h"

/*
 * Sorts the routes based upon their route length.
 */
bool sortCriteria(std::pair<int *, double> A, std::pair<int *, double> B) {
    return (A.second > B.second);
}

/*
 * Selects the best x number of children based upon route length.
 */
int** Selection::greedySelection(int** childPopulation, int childPopulationCounter,int sizeOfPopulation) {
    auto children = new std::vector<std::pair<int *, double>>;
    int** parentPopulation = new int*[sizeOfPopulation];
     for (int i = 0; i < sizeOfPopulation; ++i) {
         parentPopulation[i] = new int[NUM_OF_CUSTOMERS+1];
     }

    for (int popCounter = 0; popCounter < childPopulationCounter; ++popCounter)
        children->push_back({childPopulation[popCounter], GenerateTour::getBasicLength(childPopulation[popCounter])});

    std::sort(children->begin(), children->end(), sortCriteria);

    for (int popCounter = 0; popCounter < sizeOfPopulation; ++popCounter) {
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
            parentPopulation[popCounter][i] = children->back().first[i];
        }
//        delete[] children->back().first;
        children->pop_back();
    }

//    for (auto &i : *children) {
//        delete[] children->back().first;
//        children->pop_back();
//    }
     for (int i = 0; i < childPopulationCounter; ++i) {
         delete[] childPopulation[i];
         childPopulation[i] = nullptr;
     }

    return parentPopulation;
}

int** Selection::firstHalf(int** childPopulation, int childPopulationCounter,int sizeOfPopulation){
    auto children = new std::vector<std::pair<int *, double>>;
    int** parentPopulation = new int*[sizeOfPopulation];

    for (int popCounter = 0; popCounter < childPopulationCounter; ++popCounter)
        children->push_back({childPopulation[popCounter], GenerateTour::getBasicLength(childPopulation[popCounter])});

    std::sort(children->begin(), children->end(), sortCriteria);

    for (int popCounter = 0; popCounter < sizeOfPopulation; ++popCounter) {
        parentPopulation[popCounter] = children->back().first;
        children->pop_back();
    }

    return parentPopulation;
}

int Selection::calculateHammingDistance(const int * parent, const int * child) {
    int hamming = 0;
    for (int index = 0; index <= NUM_OF_CUSTOMERS; ++index) {
//        printf("%d:%d, ",parent[index],child[index]);
        if(parent[index] != child[index])
            hamming++;
    }
//    printf("\n");
    return hamming;
}

int** Selection::generateHammingDistanceArray(int** childPopulation,int childPopulationCounter, int** firstHalf,int sizeOfPopulation){
    int** hammingTable = new int*[childPopulationCounter];
    for (int i = 0; i < childPopulationCounter; ++i) {
        hammingTable[i] = new int[sizeOfPopulation];
        for (int j = 0; j < sizeOfPopulation; ++j) {
            hammingTable[i][j] = 0;
        }
    }
//    printf("child = %d, pop = %d\n",childPopulationCounter,sizeOfPopulation);

    for (int childPopIndex = 0; childPopIndex < childPopulationCounter; ++childPopIndex) {
        for (int parentPopIndex = 0; parentPopIndex < sizeOfPopulation; ++parentPopIndex) {
            int HD = calculateHammingDistance(firstHalf[parentPopIndex],childPopulation[childPopIndex]);
            if (HD > 0) {
                hammingTable[childPopIndex][parentPopIndex] = HD;
            }
            else{
                for (int parentPopIndexTwo = 0; parentPopIndexTwo < sizeOfPopulation; ++parentPopIndexTwo) {
                    hammingTable[childPopIndex][parentPopIndexTwo] = 0;
                }
                break;
            }
        }
    }
//    printf("DONE\n");

    return hammingTable;
}

int** Selection::correlativeFamilyBasedSelection(int** childPopulation, int childPopulationCounter,int sizeOfPopulation){
//    for (int i = 0; i < sizeOfPopulation; ++i) {
//        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
//            printf("-%d, ",childPopulation[i][j]);
//        }printf("\n");
//    }

    int** firstHalf = Selection::firstHalf(childPopulation,childPopulationCounter,(sizeOfPopulation)/2);
    int** parentPopulation = new int*[sizeOfPopulation];
    for (int i = 0; i < sizeOfPopulation; ++i) {
        parentPopulation[i] = new int[NUM_OF_CUSTOMERS+1];
    }
    for (int i = 0; i < (sizeOfPopulation+1)/2; ++i) {
        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
            parentPopulation[i][j] = firstHalf[i][j];
        }
    }
    int** hammingDistance = generateHammingDistanceArray(childPopulation,childPopulationCounter,firstHalf,(sizeOfPopulation)/2);

//    for (int i = 0; i < childPopulationCounter; ++i) {
//        for (int j = 0; j < sizeOfPopulation/2; ++j) {
//            printf("%d,%d = %d\n",i,j,hammingDistance[i][j]);
//        }
//    }

    int popCounter = (sizeOfPopulation)/2;
    for (int popIndex = 0; popIndex < sizeOfPopulation/2; ++popIndex) {
//        printf("Pop:%d\n",popIndex);
        int mostDiverseIndex = -1;
        int mostDiverse = -1;
        for (int childIndex = 0; childIndex < childPopulationCounter; ++childIndex) {
            if(hammingDistance[childIndex][popIndex] == 0){
                continue;
            }
            else{
                if(mostDiverse < hammingDistance[childIndex][popIndex]){
                    mostDiverseIndex = childIndex;
                    mostDiverse = hammingDistance[childIndex][popIndex];
                }
            }
        }
//        for (int i = 0; i < childPopulationCounter; ++i) {
//            for (int j = 0; j < sizeOfPopulation/2; ++j) {
//                printf("%d,%d = %d\n",i,j,hammingDistance[i][j]);
//            }
//        }
//        printf("end find Diverse\n");
//        printf("%d %d\n",mostDiverseIndex,mostDiverse);
        if(mostDiverseIndex == -1){
            mostDiverseIndex = 0;
        }
        for (int i = 0; i < sizeOfPopulation/2; ++i) {
            hammingDistance[mostDiverseIndex][i] = 0;
        }
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
            parentPopulation[popCounter][i] = childPopulation[mostDiverseIndex][i];
        }
        popCounter++;
    }
    if(sizeOfPopulation%2 != 0){
        int mostDiverseIndex = -1;
        int mostDiverse = 0;
        for (int childIndex = 0; childIndex < childPopulationCounter; ++childIndex) {
            if(hammingDistance[childIndex][0] == 0){
                continue;
            }
            else{
                if(mostDiverse < hammingDistance[childIndex][0]){
                    mostDiverseIndex = childIndex;
                    mostDiverse = hammingDistance[childIndex][0];
                }
            }
        }
        for (int i = 0; i < sizeOfPopulation/2; ++i) {
            hammingDistance[mostDiverseIndex][i] = 0;
        }
        for (int i = 0; i <= NUM_OF_CUSTOMERS; ++i) {
            parentPopulation[popCounter][i] = childPopulation[mostDiverseIndex][i];
        }
        popCounter++;
    }

    for (int childIndex = 0; childIndex < childPopulationCounter; ++childIndex) {
        bool childUsed = false;
        for (int popIndex = 0; popIndex < sizeOfPopulation; ++popIndex) {
            if (childPopulation[childIndex] == parentPopulation[popIndex]) {
                childUsed = true;
                break;
            }
        }
        if (!childUsed) {
//            delete[] childPopulation[childIndex];
//            childPopulation[childIndex] = nullptr;
        }
    }

    for (int i = 0; i < childPopulationCounter; ++i) {
        delete[] childPopulation[i];
        childPopulation[i] = nullptr;
    }

//        for (int i = 0; i < sizeOfPopulation; ++i) {
//        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
//            printf("%d, ",parentPopulation[i][j]);
//        }printf("\n");
//    }

    delete[] firstHalf;
    delete[] hammingDistance;
    return parentPopulation;
}