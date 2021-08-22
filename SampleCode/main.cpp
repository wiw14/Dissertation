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

auto getParameters(int input){
    auto vars = new vector<double>();
    if(input == 4){
        int knn;
        printf("Enter Number of Neighbours (KNN): ");
        cin >> knn;
        vars->push_back(knn);
    } else if (input == 5 || (7 <= input && input <=10) ){
        int nAnts,iter,probSize;
        double pheroDec,q,al,be;
        printf("Enter Number of Ants: ");
        cin >> nAnts;
        printf("Enter Number of Iterations: ");
        cin >> iter;
        printf("Enter Size of Probability (Default = 2): ");
        cin >> probSize;
        printf("Enter Value for Pheromone Decrease: ");
        cin >> pheroDec;
        printf("Enter Value for Q (Default = 1): ");
        cin >> q;
        printf("Enter Value for Alpha: ");
        cin >> al;
        printf("Enter Value for Beta: ");
        cin >> be;

        vars->push_back(nAnts);
        vars->push_back(iter);
        vars->push_back(probSize);
        vars->push_back(pheroDec);
        vars->push_back(q);
        vars->push_back(al);
        vars->push_back(be);

        if(input == 9){
            double pb;
            printf("Enter Value for pBest (Default = 0.05): ");
            cin >> pb;
            vars->push_back(pb);
        }

    }
    else if(input == 6){
        int popSize,gen,numMut;
        printf("Enter Size of Populations: ");
        cin >> popSize;
        printf("Enter Number of Generations: ");
        cin >> gen;
        printf("Enter Number of Mutations (1/x e.g. Enter 100 = 1/100): ");
        cin >> numMut;

        vars->push_back(popSize);
        vars->push_back(gen);
        vars->push_back(numMut);

        int selection,crossover,mutation;
        printf("Select Selection Operator:\n1. Truncation Selection\n2. Correlated Family-Based Selection\n");
        cin >> selection;
        printf("Select Crossover Operator:\n1. Partially Mapped Crossover\n2. Partition Crossover\n");
        cin >> crossover;
        if(crossover == 1) {
            printf("Select Mutation Operator:\n1. Random Mutation\n2. Local Search Mutation\n");
            cin>>mutation;
        }else{
            mutation = 0;
        }

        vars->push_back(selection);
        vars->push_back(crossover);
        vars->push_back(mutation);
    }
    int randomSearchIterations, twoOptIterations;
    printf("Enter Number of Random Search Iterations: ");
    cin >> randomSearchIterations;
    printf("Enter Number of Two Opt Iterations: ");
    cin >> twoOptIterations;

    vars->push_back(randomSearchIterations);
    vars->push_back(twoOptIterations);

    return vars;
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
    auto vars = getParameters(input);
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
            run_heuristic(input,vars);
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
