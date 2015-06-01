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

#include "main.h"
#include "Problem.h"

using namespace std;

int main(int argc, char *argv[]) {
    // Startup random seed
    srand((unsigned int) time(NULL));

    // Assign file argument if provided
    string inName;
    if (argc >= 2) inName = argv[1];

    // Create problem
    Problem prob(inName);

    // Get initial solution
    prob.getInitial();

    // Solve by Tabu
    prob.solveByVNS();

    cout << prob.solution.value;

    return 0;
}
