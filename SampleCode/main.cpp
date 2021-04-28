#include<iostream>
#include<stdlib.h>
#include<limits.h>

#include "Framework/EVRP.hpp"
#include "Framework/heuristic.hpp"
#include "Framework/stats.hpp"
using namespace std;


/*initialiazes a run for your heuristic*/
void start_run(int r){
    srand(r); //random seed
    init_evals();
    init_current_best();
}

/*gets an observation of the run for your heuristic*/
void end_run(int r){
//    cout << "Run: " << r << " with random seed " << r << endl;
    get_mean(r-1,get_current_best()); //from stats.h
    cout << "End of run " << r << " with best solution quality " << get_current_best() << " total evaluations: " << get_evals()  << endl;
//    cout << " " << endl;
}

/*
 * Sets the termination conidition for your heuristic
 */
bool termination_condition(void) {

    bool flag;
    if(get_evals() >= TERMINATION){
        flag = true;
    }
    else
        flag = false;

    return flag;
}

/*
 * Displays selection menu for metaheuristics.
 */
int printMenu(){
    printf("1. Greedy Heuristic\n2. Random heuristic\n3. Dijkstra Heuristic\n4. KNN Heuristic\n5. Ant Colony Optimisation on each Clusters\n6. Genetic Algorithm\n7. Ant Colony Optimisation\n8. Ant Colony Optimisation Including Charging Stations\n9. Max Min Ant Colony Optimisation\n10. Ant Colony Optimisation between Clusters\nWhich Metaheuristic:\n");
    int var;
    std::cin >> var;
    return var;
}

/*
 * Prints results table at the end.
 */
void printDataTable(double** data,int numRun){
    double avgScore = 0, avgEval = 0, avgTime = 0;
    printf("%4s|%15s|%15s|%10s\n","Run","Score","Eval","Time");
    for (int run = 0; run < numRun; ++run) {
        printf("%4d|%15f|%15f|%10f\n", run + 1, data[run][0], data[run][1], data[run][2]);
        avgScore += data[run][0]; avgEval += data[run][1]; avgTime += data[run][2];
    }
    printf("%4s|%15f|%15f|%10f\n","Avg.",(avgScore/numRun),(avgEval/numRun),(avgTime/numRun));
}
/****************************************************************/
/*                Main Function                                 */
/****************************************************************/
int main(int argc, char *argv[]) {

    int run;
    /*Step 1 - Read problem file.*/
    problem_instance = argv[1];
    read_problem(problem_instance);

    /*Step 2 - Open stats files.*/
    open_stats();
    openTourFile();
    int input = printMenu();
    int NumRuns = 20;
    auto runData = new double*[NumRuns];
    for(run = 1; run <= NumRuns; run++){
        runData[run-1] = new double[3];
        printf("Run %d...\n",run);
        /*Step 3 - Start Run*/
        start_run(run);
        //Initialize your heuristic here
        initialize_heuristic();

        /*Step 4 - Run heuristic*/
        int count = 1;
        while(!termination_condition()){
            //Execute your heuristic
            openRunDataFile(run);
            run_heuristic(input);
            closeRunDataFile(run);
        }

        runData[run-1][0] = get_current_best();
        runData[run-1][1] = get_evals();


        storeTour(run);

        /*Step 5 - End run*/
        end_run(run);  //store the best solution quality for each run
        runData[run-1][2] = end_heuristic();
    }
    /*Step 6 - Save best results to stats files.*/
    close_stats(); //close text files to calculate the mean result from the 20 runs stats.h
    closeTourFile();

    //Step 7 - Free memory
    free_stats();
    free_heuristic();
    free_EVRP();

    printDataTable(runData,NumRuns);

    for (int runs = 0; runs < NumRuns; ++runs)
        delete[] runData[runs];
    delete[] runData;
    return 0;
}
