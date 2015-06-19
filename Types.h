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
    long nSolution;
    double value;
    vector<long> elements;
    vector<long> notChosen;
    Problem *problem;

    Solution();
    Solution(Problem *);
    Solution(const Solution &);
    Solution& operator=(const Solution &);

    void initGreedy();
    void initRandom();

    void doLocalSearch();

    void replaceIndexByIndex(long, long);
    void replaceIndexByValue(long, long);
    void recalcValue(long, long, long);
};

class GeneticSolution {
public:
    long nSolution;
    double value;
    vector<bool> bitmap;
    Problem *problem;

    GeneticSolution();
    GeneticSolution(Problem *);
    GeneticSolution(const GeneticSolution &);
    GeneticSolution& operator=(const GeneticSolution &);

    void toggle(long);
    static vector<long> rouletteSelection(const vector<GeneticSolution> &, long);
    static void geneticCrossover(GeneticSolution &, GeneticSolution &);
    
};

class Problem {
public:
    long nNodes;
    long nEdges;
    long nSolution;

    Solution solution;
    vector<double> matrix;
    vector<pair<long,double>> potentials;

    Problem();
    Problem(const Problem &);
    Problem &operator=(const Problem &p);
    static Problem fromFile(string);

    void initGreedy();
    void initRandom();

    void solveByLocalSearch();
    void solveByVNS();
    void solveByTabu();
    void solveByGenetic();
    
    int difference(Solution, Solution);
    Solution combineSS(Solution, Solution);
    vector<Solution> generateRefSetFromP(vector<Solution>, int);
    vector<Solution> generateP(int);
    void solveByScatter();

    long matIndex(long, long);
    double getEdge(long, long);
};

#endif // CI5652_MDP_TYPES_H
