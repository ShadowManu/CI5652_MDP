//
// Created by shadowmanu on 28/05/15.
//

#ifndef CI5652_MDP_PROBLEM_H
#define CI5652_MDP_PROBLEM_H

#include <string>
#include <utility>
#include <memory>
#include <vector>

using namespace std;

class Problem {
    int nNodes;
    int nEdges;
    int nSolution;

    vector<double> matrix;
    vector<double>potentials;
    vector<int> solution;

    int matIndex(int, int);
public:
    Problem(string filename);
    void solveProblem();
};

#endif //CI5652_MDP_PROBLEM_H
