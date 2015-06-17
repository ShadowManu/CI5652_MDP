/**
 * Problem and Solution class definitions
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */

#ifndef CI5652_MDP_TYPES_H
#define CI5652_MDP_TYPES_H

#include <string>
#include <vector>

using namespace std;

class Problem;

class Solution {
public:
    int nSolution;
    double value;
    vector<int> elements;
    vector<int> notChosen;
    Problem *problem;

    Solution();
    Solution(Problem *p);
    Solution(const Solution &);
    Solution& operator=(const Solution &);

    void initGreedy();
    void initRandom();

    void doLocalSearch();

    void replaceIndexByIndex(int, int);
    void replaceIndexByValue(int, int);
    void recalcValue(int, int, int);
};

class Problem {
public:
    int nNodes;
    int nEdges;
    int nSolution;

    Solution solution;
    vector<double> matrix;
    vector<pair<int,double>> potentials;

    Problem();
    Problem(const Problem &);
    Problem &operator=(const Problem &p);
    static Problem fromFile(string);

    void initGreedy();
    void initRandom();

    void solLocalSearch();
    void solveByVNS();
    void solveByTabu();

    int matIndex(int, int);
    double getEdge(int, int);
};

#endif // CI5652_MDP_TYPES_H