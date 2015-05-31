//
// Created by shadowmanu on 28/05/15.
//

#ifndef CI5652_MDP_PROBLEM_H
#define CI5652_MDP_PROBLEM_H

#include <string>
#include <utility>
#include <memory>
#include <vector>
#include <unordered_set>

using namespace std;

class Problem;

class Solution {
public:
    int nSolution;
    vector<int> elements;
    vector<int> notChosen;

    Problem *problem;
    Solution(Problem *p);
    double calcSolutionValue();
    void initialSolution();
    void getInitial();

    void solLocalSearch();
};

class Problem {
public:
    int nNodes;
    int nEdges;
    int nSolution;

    vector<double> matrix;
    vector<double>potentials;
    Solution solution;

    int matIndex(int, int);

    Problem(string);
    double getEdge(int, int);
    void solve();
    void getInitial();
    void solLocalSearch();
};

#endif //CI5652_MDP_PROBLEM_H
