//
// Created by shadowmanu on 28/05/15.
//

#include <string>
#include <istream>

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


    return 0;
}
