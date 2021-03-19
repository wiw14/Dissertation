#include "ACOOnClusters.h"

int *generateCostTable(int sizeOfAlphabet, std::vector<int *> clusters, int *sizes) {
    auto **costTable = new double *[sizeOfAlphabet];
    for (int i = 0; i < sizeOfAlphabet; ++i) {
        costTable[i] = new double[sizeOfAlphabet];
        for (int j = 0; j < sizeOfAlphabet; ++j) {
            costTable[i][j] = 0;
        }
    }

    for (int i = 0; i < sizeOfAlphabet; ++i) {
        for (int j = 0; j < sizeOfAlphabet; ++j) {
            if (i != j) {
                costTable[i][j] = get_distance(clusters.at(i)[sizes[i] - 1], clusters.at(j)[0]);
            }
        }
    }

    auto min = std::pair<int, int>();
    double minVal = INT_MAX;
    for (int i = 0; i < sizeOfAlphabet; ++i) {
        for (int j = 0; j < sizeOfAlphabet; ++j) {
            if (costTable[i][j] < minVal) {
                minVal = costTable[i][j];
                min.first = i;
                min.second = j;
            }
        }
    }

    int *finalRoute = new int[NUM_OF_CUSTOMERS + 1];
    int count = 0;
    finalRoute[count++] = DEPOT;
    auto addToRoute = [&]() {
        for (int customers = 0; customers < sizes[min.second]; ++customers) {
            finalRoute[count++] = clusters.at(min.second)[customers];
        }
    };
//    addToRoute();
    bool* visited = new bool[sizeOfAlphabet];
    for (int i = 0; i < sizeOfAlphabet; ++i) {
        visited[i] = false;
    }

    for (int customers = 0; customers < sizes[min.first]; ++customers) {
        finalRoute[count++] = clusters.at(min.first)[customers];
    }
    for (int i = 0; i < sizeOfAlphabet-1; ++i) {
        visited[min.first] = true;
        visited[min.second] = true;
        minVal =INT_MAX;
        int tempIndex = min.second;

        for (int j = 0; j < sizeOfAlphabet; ++j) {
            if(!visited[j] && costTable[tempIndex][j] < minVal){
                minVal = costTable[tempIndex][j];
                min.first = tempIndex;
                min.second = j;
            }
        }
        addToRoute();
    }

    for (int i = 0; i < sizeOfAlphabet; ++i) {
        delete[] costTable[i];
    }
    delete[] costTable;
    delete[] visited;
    return finalRoute;
}

void ACOOnClusters() {
    int numAnts = 3, iterations = 18, probabilityArraySize = 2, twoOptIteration = 3, randomSearchIteration = 3;
    double pheromoneDecrease = 0.9, Q = 1, alpha = 0.9, beta = 0.7;
//    int numAnts = 4, iterations = 15, probabilityArraySize = 2, twoOptIteration = 3, randomSearchIteration = 3;
//    double pheromoneDecrease = 0.9, Q = 1, alpha = 0.9, beta = 0.9;

    auto cluster = new Cluster();

    auto clusters = std::vector<int *>();
    int *clusterSizes = new int[cluster->numOfClusters];
    for (int clusterIndex = 0; clusterIndex < cluster->numOfClusters; ++clusterIndex) {
        if(cluster->clusters->at(clusterIndex)->sizeOfCluster > 1) {
            auto clusterACO = new ClusterACO(numAnts, pheromoneDecrease, Q, probabilityArraySize, alpha, beta,
                                             twoOptIteration, randomSearchIteration,
                                             cluster->clusters->at(clusterIndex)->customers,
                                             cluster->clusters->at(clusterIndex)->sizeOfCluster);

//        for (int i = 0; i < cluster->clusters->at(clusterIndex)->sizeOfCluster; ++i)
//            printf("%d, ", cluster->clusters->at(clusterIndex)->customers[i]);
//        printf("\n");

            clusterACO->optimize(iterations);
            int *route = clusterACO->returnResults();
            clusterSizes[clusterIndex] = cluster->clusters->at(clusterIndex)->sizeOfCluster;
            clusters.push_back(route);

//        for (int i = 0; i < cluster->clusters->at(clusterIndex)->sizeOfCluster; ++i)
//            printf("%d, ", route[i]);
//        printf("\n");
//        printf("\n");

            delete clusterACO;
        }
        else{
            clusterSizes[clusterIndex] = cluster->clusters->at(clusterIndex)->sizeOfCluster;
            clusters.push_back(cluster->clusters->at(clusterIndex)->customers);
        }
    }

    int* r = generateCostTable(cluster->numOfClusters, clusters, clusterSizes);

//    for (int i = 0; i <=  NUM_OF_CUSTOMERS; ++i) {
//        printf("%d, ",r[i]);
//    }printf("\n");
    auto* LS = new localSearch(3,3);
    for (int i = 0; i < 3; ++i) {
//        int* temp = new int[NUM_OF_CUSTOMERS+1];
//        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
//            temp[j] = r[j];
//        }
        LS->randomPheromoneLocalSearchWithTwoOpt(r);
//        bool change = false;
//        for (int j = 0; j <= NUM_OF_CUSTOMERS; ++j) {
//            if(r[j] != temp[j])
//                change = true;
//        }
//        delete[]temp;
//        if(change)
//            break;
    }
//    GenerateTour::getRouteLength(r);

    delete LS;
    delete cluster;
}