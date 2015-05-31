//
// Created by shadowmanu on 28/05/15.
//

#include "Problem.h"
#include <iostream>
#include <fstream>

using namespace std;

Problem::Problem(string filename) : nNodes(0), nSolution(0) {
    // Open input stream from filename
    // and fallback to standard input if string is empty
    shared_ptr<istream> in;
    if (filename.empty()) {
        in.reset(&cin,[](...){});
    } else {
        in.reset(new ifstream(filename));
    }

    // Read nNodes and solutionSize
    *in >> nNodes >> nSolution;
    nEdges = (nNodes*(nNodes - 1)) / 2;

    // Set sizes for matrix, potentials and solution
    matrix.resize((unsigned long) (nNodes * nNodes));
    potentials.resize((unsigned long) nNodes);
    solution.resize((unsigned long) nSolution);

    // Read edges
    {
        int i, from, to; double length;
        for (i = 0; i < nEdges; i++) {
            *in >> from >> to >> length;
            potentials[from] += length;
            potentials[to] += length;
            matrix[matIndex(from, to)] = length;
            matrix[matIndex(to, from)] = length;
        }
    }
}

int Problem::matIndex(int a, int b) {
    return nNodes * a + b;
}

void Problem::solveProblem() {


}