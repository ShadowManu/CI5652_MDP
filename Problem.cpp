/**
 * Problem and Solution implementation
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_set>

#include "Problem.h"

using namespace std;

/*********** SOLUTION IMPLEMENTATION **********/

/**
 * Solution Constructor
 */
Solution::Solution(Problem *p) : problem(p), nSolution(0), value(0) {}

/**
 * Finish solution startup (including initialSolution)
 */
void Solution::getInitial() {
    nSolution = problem->nSolution;
    initialSolution();
}

/**
 * Initial solution based on problem's potentials
 */
void Solution::initialSolution() {
    // Choose first nSolution as initial solution
    for (int i=0; i<nSolution; i++)
        elements.push_back(problem->potentials[i].first);

    // Calculate value
    for (int i=0; i<nSolution; i++) {
        for (int j=i+1;j<nSolution; j++) {
            value += problem->getEdge(elements[i],elements[j]);
        }
    }

    // Classify others as not chosen
    for (int i=nSolution; i<problem->nNodes; i++)
        notChosen.push_back(problem->potentials[i].first);
}

/**
 *  Local search implementation
 */
void Solution::solLocalSearch() {
    const int ALLOWED_REPETITIONS = (int) ((problem->nNodes - nSolution) * 0.05);
    double currentDistance = value;

    // For each node from right to left
    for (int i=nSolution-1; i>=0; i--) {
        unordered_set<int> tested;
        int originalNode = elements[i];
        int bestNode = 0;
        int index = 0;
        bool modified = false;

        // Search best replacing node from notChosen ones
        // with a limited number of repetitions
        for (int j = 0; j < ALLOWED_REPETITIONS && tested.size() < notChosen.size();) {

            // Choose an untested index from notChosen
            do {
                index = (int) (rand() % notChosen.size());
            } while (tested.find(index) != tested.end());
            tested.insert(index);

            // Replace node
            replaceIndexByIndex(i, index);

            // Check if its an improvement
            double newDistance = value;
            if (currentDistance < newDistance) {
                currentDistance = newDistance;
                bestNode = elements[i];
                modified = true;
                j = 0;
                // Else, keep searching
            } else {
                j++;
            }
        }
        // If modified, put best node
        if (modified) replaceIndexByValue(i, bestNode);

        // Else, put original back
        else replaceIndexByValue(i, originalNode);
    }
}

/**
 * Replace a solution node by index, using index from not chosen
 */
void Solution::replaceIndexByIndex(int solIndex, int ncIndex) {
    int oldNode = elements[solIndex];
    int newNode = notChosen[ncIndex];

    // Swap nodes
    elements[solIndex] = newNode;
    notChosen[ncIndex] = oldNode;

    // Recalculate solution
    recalcValue(solIndex, oldNode, newNode);
}

/**
 * Replace a solution node by index, using value from not chosen
 */
void Solution::replaceIndexByValue(int solIndex, int newNode) {
    int oldNode = elements[solIndex];
    int ncIndex;
    for (ncIndex = 0; ncIndex<notChosen.size(); ncIndex++)
        if (notChosen[ncIndex] == newNode) break;

    // Swap nodes
    elements[solIndex] = newNode;
    notChosen[ncIndex] = oldNode;

    // Recalculate solution
    recalcValue(solIndex, oldNode, newNode);

}

/**
 * Swaps two values in an index, updating solution value
 */
void Solution::recalcValue(int index, int oldNode, int newNode) {
    // Recalculate for each diferent node the new edge
    for (int i=0; i<nSolution; i++) {
        if (i != index) {
            value -= problem->getEdge(elements[i], oldNode);
            value += problem->getEdge(elements[i], newNode);
        }
    }
}

/**
 * Returns the objective function value of the problem
 */
double Solution::calcValueFromScratch() {
    double total = 0;

    for (int i=0; i<nSolution; i++) {
        for (int j=i+1; j<nSolution; j++){
            total += problem->getEdge(elements[i],elements[j]);
        }
    }

    return total;
}

/********** PROBLEM IMPLEMENTATION **********/

/**
 * Comparator for potential sorting
 */
bool potentialComp(pair<int,double> a, pair<int,double> b) {
    return a.second > b.second;
}

/**
 * Problem constructor
 */
Problem::Problem(string filename) : solution(this) {
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

    // Set size for matrix and temporal potentials;
    matrix.resize((unsigned long) (nNodes * nNodes));
    vector<int> temp_potentials((unsigned long) nNodes);

    // Read edges
    {
        int i, from, to; double length;
        for (i = 0; i < nEdges; i++) {
            *in >> from >> to >> length;
            temp_potentials[from] += length;
            temp_potentials[to] += length;
            matrix[matIndex(from, to)] = length;
            matrix[matIndex(to, from)] = length;
        }
    }

    // Fill and sort potentials
    for (int i=0; i<nNodes; i++)
        potentials.push_back(pair<int,double>(i,temp_potentials[i]));
    sort(potentials.begin(),potentials.end(),potentialComp);
}

/**
 * Obtain problem initial solution
 */
void Problem::getInitial() {
    solution.getInitial();
}

/**
 * Improve solution using localSearch()
 */
void Problem::solLocalSearch() {
    solution.solLocalSearch();
}

/**
 * Metaheuristics solution
 */
void Problem::solve() {
    // TODO METAHEURISTICS
}

/**
 * Get edge weigth from source and dest
 */
double Problem::getEdge(int i, int j) {
    return matrix[matIndex(i,j)];
}

/**
 * Get unidimensional index based on two dimensins
 */
int Problem::matIndex(int i, int j) {
    return nNodes * i + j;
}
