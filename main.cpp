/**
 * Main implementation of MDP solver
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */
#include <string>
#include <istream>
#include <iostream>

#include <ctime>
#include <cstring>

#include "main.h"

using namespace std;

int main(int argc, char *argv[]) {
    // Arg 0: program name
    // Arg 1: problem filename
    // Arg 2: greedy or random
    // Arg 3: metaheuristic name
    // Arg 4: Random seed

    // Startup random seed
    srand((unsigned int) ((argc < 5) ? time(NULL) : stoi(argv[4])));

    // Read problem data
    Problem problem(Problem::fromFile(argv[1]));

    // Start timer
    clock_t begin = clock();

    // Choose initial solution strategy
    if (strcmp(argv[2],"greedy") == 0)
        problem.initGreedy();
    if (strcmp(argv[2],"random") == 0)
        problem.initRandom();

    // Save initial solution
    double initialSolution = problem.solution.value;

    // Choose metaheuristic strategy
    if (strcmp(argv[3],"localsearch") == 0)
        problem.solveByLocalSearch();
    if (strcmp(argv[3],"ils") == 0)
        problem.solveByILS();
    if (strcmp(argv[3],"tabu") == 0)
        problem.solveByTabu();
    if (strcmp(argv[3],"genetic") == 0)
        problem.solveByGenetic();
    if (strcmp(argv[3],"scatter") == 0)
        problem.solveByScatter();

    // Save final solution
    double finalSolution = problem.solution.value;

    // End timer
    clock_t end = clock();
    // Calculate number of seconds
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    // Print results (initial, final, timing)
    cout.precision(15);
    cout << initialSolution << " " << finalSolution << " " << elapsed_secs << endl;

    return 0;
}
