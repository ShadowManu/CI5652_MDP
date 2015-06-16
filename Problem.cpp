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
#include <queue>

#include "Types.h"

using namespace std;

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
void Problem::solveByVNS() {
    const int INIT_SIZE_PERTURBATION = (int) (nSolution * 0.1); // TODO CHECK
    const int MAX_ITERATIONS_VNS = 100000; // TODO CHECK
    const int MAX_TRIES_K = 10; // TODO CHECK
    int k = INIT_SIZE_PERTURBATION;  // Size for shaking

    solution.solLocalSearch();
    Solution workingSolution(solution);

    int triesWithK = 0;

    for (int numIteration=0; (numIteration < MAX_ITERATIONS_VNS) && (k<nSolution); numIteration++) {

        // Shake k nodes in working solution
        for (int i=0; i<k; i++) {
            unordered_set<int> solutionIndices;
            unordered_set<int> notChosenIndices;
            int solIndex;
            int ncIndex;

            // Choose a random index from solution
            do {
                solIndex = (rand() % nSolution);
            } while (solutionIndices.find(solIndex) != solutionIndices.end());
            solutionIndices.insert(solIndex);

            // Choose a random index from not chosen
            do {
                ncIndex = (int) (rand() % workingSolution.notChosen.size());
            } while (notChosenIndices.find(ncIndex) != notChosenIndices.end());
            notChosenIndices.insert(ncIndex);

            // Swap the values in the indices
            workingSolution.replaceIndexByIndex(solIndex,ncIndex);
        }

        // Improve with Local Search;
        workingSolution.solLocalSearch();

        // If it is a better solution
        if (solution.value < workingSolution.value) {
            solution = workingSolution;
            k = INIT_SIZE_PERTURBATION;
            triesWithK = 0;
        }
        // Otherwise,
        else {
            // If exceeded tries
            if (triesWithK >= MAX_TRIES_K) {
                k++;
                triesWithK = 0;
            }
            else {
                triesWithK++;
            }
            workingSolution = solution;
        }
    }
}

/**
 * Solve problem using taboo search
 */
void Problem::solveByTabu() {

    const int k = (int) (nSolution * 0.1); // TODO CHECK
    const int MAX_ITERATIONS = 10000000; // TODO CHECK
    const int MAX_TRIES = 1000; // TODO CHECK
    const int JAIL_TIME = 30;

    solution.solLocalSearch();
    Solution workingSolution(solution);

    unordered_set<int> tabuIndices;
    vector<int> tabuTimers(solution.notChosen.size(), 0);

    int noChange = 0;
    for (int numIterations = 0; numIterations < MAX_ITERATIONS && noChange < MAX_TRIES; numIterations++) {
        unordered_set<int> notChosenIndices;
        unordered_set<int> solutionIndices;

        // Choose k from solution
        for (int i=0; i<k; i++) {
            int solIndex;

            // Choose a random index from solution
            do {
                solIndex = (rand() % nSolution);
            } while (solutionIndices.find(solIndex) != solutionIndices.end());
            solutionIndices.insert(solIndex);
        }

        // If k nodes can be chosen (not many tabooes)
        if (solution.notChosen.size() - tabuIndices.size() >= k) {

            // Choose the k nodes from not chosen (and not in taboo)
            for (int i=0; i<k; i++) {
                int ncIndex;
                do {
                    ncIndex = (int) (rand() % workingSolution.notChosen.size());
                } while (notChosenIndices.find(ncIndex) != notChosenIndices.end() ||
                        tabuIndices.find(ncIndex) != tabuIndices.find(ncIndex));
                notChosenIndices.insert(ncIndex);
            }

        // Otherwise, we complete with taboo
        }
        else {
            int possible = (int) (solution.notChosen.size() - tabuIndices.size());

            // The ones we can take normally
            for (int j=0; j<possible; j++) {
                int ncIndex;
                do {
                    ncIndex = (int) (rand() % workingSolution.notChosen.size());
                } while (notChosenIndices.find(ncIndex) != notChosenIndices.end() ||
                         tabuIndices.find(ncIndex) != tabuIndices.find(ncIndex));
                notChosenIndices.insert(ncIndex);
            }

            // The ones we complete with taboo ones
            for (int j=0; j<k-possible; j++) {
                int ncIndex;
                do {
                    ncIndex = (int) (rand() % workingSolution.notChosen.size());
                } while (notChosenIndices.find(ncIndex) != notChosenIndices.end());
                notChosenIndices.insert(ncIndex);
                tabuIndices.erase(ncIndex); // Free from taboo
            }
        }

        // Replace the k nodes
        for (auto it1 = solutionIndices.begin(), it2 = notChosenIndices.begin(); it1 != solutionIndices.end(); it1++, it2++) {
            workingSolution.replaceIndexByIndex(*it1, *it2);
        }

        // If it is a better solution
        if (solution.value < workingSolution.value) {
            solution = workingSolution;
            solution.solLocalSearch();
            tabuIndices.clear();
            fill(tabuTimers.begin(),tabuTimers.end(),0);
            noChange = 0;

        // If it isn't
        } else {
            noChange++;
            // Rewind the working solution
            workingSolution = solution;

            // Decrease jail time
            for (int i=0; i<tabuTimers.size(); i++) {
                if (tabuTimers[i] > 0) {
                    tabuTimers[i]--;
                }
            }

            // Mark used nodes as taboo
            for (auto it = notChosenIndices.begin(); it != notChosenIndices.end(); it++) {
                tabuIndices.insert(*it);
                tabuTimers[*it] = JAIL_TIME;
            }
        }

        // Clear sets
        solutionIndices.clear();
        notChosenIndices.clear();
    }

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
