#include "Randoms.cpp"

class ACOv1 {
public:
    ACOv1 (int nAnts, int numCustomers,
           double alpha, double beta, double q, double ro, double taumax,
           int startDepot);
    virtual ~ACOv1 ();

    void init ();

    void connectCustomers (int cityi, int cityj);
    void setCustomerLocation (int city, double x, double y);

    void printPHEROMONES ();
    void printGRAPH ();
    void printRESULTS ();
    int* returnResults();

    void optimize (int ITERATIONS);

private:
    double distance (int cityi, int cityj);
    bool exists (int cityi, int cityc);
    bool visited (int antk, int c);
    double PHI (int customerA, int customerB, int antk);

    double length (int antk);

    int customer ();
    void route (int antk);
    int valid (int antk, int iteration);

    void updatePHEROMONES ();


    int numberOfAnts, numberOfCustomers, depot;
    double Alpha, Beta, Q, RO, Taumax;

    double bestLength;
    int *bestRoute;

    int **routes;
    double **customers, **pheromones, **deltaPheromones, **probability;

    Randoms *randoms;
};