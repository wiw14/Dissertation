#include <iostream>
#include <climits>
#include "ACOHeuristic.h"
#include "EVRP.hpp"
#include "heuristic.hpp"
#include "stats.hpp"

using namespace std;

struct snapshot {
    int numAnts;
    int iterations;
    int probabilityArraySize;
    int twoOptIteration;
    int randomSearchIteration;
    double pheromoneDecrease;
    double Q;
    double alpha;
    double beta;
};

snapshot *SNAPSHOT;

/*initialiazes a run for your heuristic*/
void start_run(int r) {

    srand(r); //random seed
    init_evals();
    init_current_best();
//    cout << "Run: " << r << " with random seed " << r << endl;
}

/*gets an observation of the run for your heuristic*/
void end_run(int r) {

    cout << "End of run " << r << " with best solution quality " << get_current_best() << " total evaluations: "
         << get_evals() << endl;
    cout << " " << endl;
}

/*sets the termination conidition for your heuristic*/
bool termination_condition(void) {

    bool flag;
    if (get_evals() >= TERMINATION) {
        flag = true;
    } else
        flag = false;

    return flag;
}

int *numAntsBound, *iterationsBound, *twoOptIterationBound, *randomSearchIterationBound;
double *pheromoneDecreaseBound, *QBound, *alphaBound, *betaBound;

void initialiseBounds() {
    numAntsBound = new int[2]{3, 5};
    iterationsBound = new int[2]{40,70};
    twoOptIterationBound = new int[2]{1, 10};
    randomSearchIterationBound = new int[2]{1, 10};
    pheromoneDecreaseBound = new double[2]{0.5, 0.9};
    QBound = new double[2]{1,30};
    alphaBound = new double[2]{0.1,3};
    betaBound = new double[2]{0.1, 3};
}


void initialiseSnapshot() {
    SNAPSHOT = new snapshot;
    SNAPSHOT->probabilityArraySize = 2;
}

void setSnapshot(int NUMANTS, int ITERATIONS, int TWOOPTITERATION, int RANDOMSEARCHITERATION, double PHEROMONEDECREASE,
                 double q, double ALPHA, double BETA) {
    SNAPSHOT->numAnts = NUMANTS;
    SNAPSHOT->iterations = ITERATIONS;
    SNAPSHOT->twoOptIteration = TWOOPTITERATION;
    SNAPSHOT->randomSearchIteration = RANDOMSEARCHITERATION;
    SNAPSHOT->pheromoneDecrease = PHEROMONEDECREASE;
    SNAPSHOT->Q = q;
    SNAPSHOT->alpha = ALPHA;
    SNAPSHOT->beta = BETA;
}


void freeBounds() {
    delete[] numAntsBound;
    delete[] iterationsBound;
    delete[] twoOptIterationBound;
    delete[] randomSearchIterationBound;
    delete[] pheromoneDecreaseBound;
    delete[] QBound;
    delete[] alphaBound;
    delete[] betaBound;
}

void copySnapShot(snapshot *original, snapshot *copy) {
    copy->numAnts = original->numAnts;
    copy->iterations = original->iterations;
    copy->probabilityArraySize = original->probabilityArraySize;
    copy->twoOptIteration = original->twoOptIteration;
    copy->randomSearchIteration = original->randomSearchIteration;
    copy->pheromoneDecrease = original->pheromoneDecrease;
    copy->Q = original->Q;
    copy->alpha = original->alpha;
    copy->beta = original->beta;
}

void displaySnapshot(snapshot *Snapshot) {
    printf("Num Ants: %d, Iterations: %d, Probability Array Size: %d, 2-Opt Iterations: %d, Random Search Iterations: %d\n",
           Snapshot->numAnts, Snapshot->iterations, Snapshot->probabilityArraySize, Snapshot->twoOptIteration,
           Snapshot->randomSearchIteration);
    printf("Pheromone Decrease: %f, Q: %f, Alpha: %f, Beta: %f\n", Snapshot->pheromoneDecrease, Snapshot->Q,
           Snapshot->alpha, Snapshot->beta);
}

int main(int argc, char *argv[]) {

    /*Step 1*/
    problem_instance = argv[1];       //pass the .evrp filename as an argument
    read_problem(problem_instance);   //Read EVRP from file from EVRP.h
    double bestDist = INT_MAX;
    auto *bestSnapshot = new snapshot;
    initialiseBounds();
    initialiseSnapshot();
    start_run(1);
    for (int I = iterationsBound[0]; I <= iterationsBound[1];) {
        for (int NA = numAntsBound[0]; NA <= numAntsBound[1]; NA++) {
            //for (int TOI = twoOptIterationBound[0]; TOI <= twoOptIterationBound[1]; ++TOI) {
            //  for (int RSI = randomSearchIterationBound[0]; RSI <= randomSearchIterationBound[1]; ++RSI) {
            for (double PD = pheromoneDecreaseBound[0]; PD <= pheromoneDecreaseBound[1];) {
                for (double Q = QBound[0]; Q <= QBound[1];) {
                    for (double A = alphaBound[0]; A <= alphaBound[1];) {
                        for (double B = betaBound[0]; B <= betaBound[1];) {

//                            initialize_heuristic(); //heuristic.h
                            setSnapshot(NA, I, 3, 3, PD, Q, A, B);
                            double total = 0.0;
                            for (int i = 1; i <= MAX_TRIALS; ++i) {

                                start_run(i);
                                initialize_heuristic();
                                ACOHeuristic(SNAPSHOT->numAnts, SNAPSHOT->iterations, SNAPSHOT->probabilityArraySize,
                                         SNAPSHOT->twoOptIteration, SNAPSHOT->randomSearchIteration,
                                         SNAPSHOT->pheromoneDecrease, SNAPSHOT->Q, SNAPSHOT->alpha, SNAPSHOT->beta);
                                total +=get_current_best();
                                if(total/(double)i > bestDist) {
                                    total = INT_MAX;
                                    free_heuristic();
                                    break;
                                }
                                free_heuristic();
                            }
                            double currentBest = total/(double)MAX_TRIALS;
                            if (currentBest < bestDist) {
                                printf("-cb-%f-- -gb-%f--\n", currentBest, bestDist);
                                bestDist = currentBest;
                                copySnapShot(SNAPSHOT, bestSnapshot);
                                displaySnapshot(SNAPSHOT);

                            }


                            B += 0.5;
                        }
                        A += 0.5;
                    }
                    Q += 10;
                }

                PD += 0.1;
            }
//                }
//            }
            I += 10;
            printf("i:%d\n",I);
        }
    }

    displaySnapshot(bestSnapshot);

    //free memory
    freeBounds();
    free_EVRP();
    delete SNAPSHOT;
    delete bestSnapshot;
    return 0;
}
