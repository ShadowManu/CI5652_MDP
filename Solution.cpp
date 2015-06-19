/**
 * Solution implementation
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */

#include <algorithm>

#include "Types.h"
#include "Helpers.h"

using namespace std;

/**
 * Default Constructor
 */
Solution::Solution() : problem(nullptr), nSolution(0), value(0) {}

/**
 * Copy Constructor using Problem
 */
Solution::Solution(Problem *p) : problem(p), nSolution(0), value(0) {}

/**
 * Copy Constructor using Solution
 */
Solution::Solution(const Solution &s) :
        problem(s.problem),
        nSolution(s.nSolution),
        value(s.value),
        elements(s.elements),
        notChosen(s.notChosen) { }

/*
 * Assignment Operator
 */
Solution& Solution::operator=(const Solution &s) {
    problem = s.problem;
    nSolution = s.nSolution;
    value = s.value;
    elements = s.elements;
    notChosen = s.notChosen;
    return *this;
}

/*
 * Compare two solution, and return number of nodes they don't share
 */
int Solution::difference(const Solution s){
    int diff = 0;
    for (int i=0; i<s.nSolution; i++){
        for (int j=0; j<s.nSolution; j++) {
            if (this.elements[i] == s.elements[j]){
                diff++;
                break;
            }
        }
    }
    return diff;
}

/*
 *  Combine two solution for ScatterSearch
 */
Solution combineSS(Solution a, Solution b){
    Solution out = initRandom();
    bool turn = true;
    
    for (int i=0; i<out.nSolution; i++){
        if (turn) {
            out.replaceIndexByValue(i, a.elements[i]);
        } else {
            out.replaceIndexByValue(i, b.elements[i]);
        }
    }

    return out;
}

/**
 * Obtain problem initial solution
 * using a greedy strategy:
 * Choose nodes with better potentials
 */
void Solution::initGreedy() {
    // Problem setup
    nSolution = problem->nSolution;

    // Choose first nSolution as initial solution
    for (auto i=0; i<nSolution; i++)
        elements.push_back(problem->potentials[i].first);

    // Calculate initial value
    for (auto i=0; i<nSolution; i++) {
        for (auto j=i+1; j<nSolution; j++) {
            value += problem->getEdge(elements[i],elements[j]);
        }
    }

    // Classify others as not chosen
    for (auto i=nSolution; i<problem->nNodes; i++)
        notChosen.push_back(problem->potentials[i].first);
}

/**
 * Obtain problem initial solution
 * using a random strategy:
 *
 */
void Solution::initRandom() {
    // Problem setup
    nSolution = problem->nSolution;

    // Create a set with all elements
    vector<long> indices = vectorRange(problem->nNodes);

    // Choose the random elements
    for (auto i=0; i<nSolution; i++)
        elements.push_back(pop_random(indices));

    // Calculate initial value
    for (int i=0; i<nSolution; i++) {
        for (int j=i+1; j<nSolution; j++) {
            value += problem->getEdge(elements[i],elements[j]);
        }
    }

    // Classify others as not chosen
    for (long i=nSolution; i<problem->nNodes; i++)
        notChosen.push_back(pop_random(indices));
}

/**
 *  Local search implementation
 */
void Solution::doLocalSearch() {
    const long NH_SIZE = (long) (notChosen.size() * (1.0/5));
    const long MAX_REPEATS = 30;

    // Original data from solution
    double bestDistance = value;

    // For each node from right to left
    for (auto i=nSolution-1; i>=0; i--) {

        // Original data from solution node
        long originalNode = elements[i], bestNode = elements[i];
        bool modified = false;

        // Choose neighborhood nodes
        vector<long> neighbors = chooseNRandomFromVector(notChosen, NH_SIZE);

        // Search best replacing node from the neighborhood
        // with a limit number of repetitions without change
        for (auto j=0, repeats=1; j<NH_SIZE && repeats<=MAX_REPEATS; j++, repeats++) {

            // Choose a node
            long iNode = pop_random(neighbors);
            // Replace node
            replaceIndexByIndex(i,iNode);

            // If its an improvement, remember it as best option
            // resetting the repetitions
            double newDistance = value;
            if (bestDistance < newDistance) {
                bestDistance = newDistance;
                bestNode = elements[i];
                modified = true;
                repeats = 0;
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
void Solution::replaceIndexByIndex(long solIndex, long ncIndex) {
    long oldNode = elements[solIndex];
    long newNode = notChosen[ncIndex];

    // Swap nodes
    elements[solIndex] = newNode;
    notChosen[ncIndex] = oldNode;

    // Recalculate solution
    recalcValue(solIndex, oldNode, newNode);
}

/**
 * Replace a solution node by index, using value from not chosen
 */
void Solution::replaceIndexByValue(long solIndex, long newNode) {
    long oldNode = elements[solIndex];
    long ncIndex;
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
void Solution::recalcValue(long index, long oldNode, long newNode) {
    // Recalculate for each diferent node the new edge
    for (int i=0; i<nSolution; i++) {
        if (i != index) {
            value -= problem->getEdge(elements[i], oldNode);
            value += problem->getEdge(elements[i], newNode);
        }
    }
}
