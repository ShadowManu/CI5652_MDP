/**
 * Problem implementation
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */

#include <iostream>
#include <fstream>
#include <memory>

#include <algorithm>
#include <unordered_set>

#include "Types.h"
#include "Helpers.h"

using namespace std;

/**
 * Default Constructor
 */
Problem::Problem() : solution(this) {}

/**
 * Copy Constructor using Problem
 */
Problem::Problem(const Problem &p) :
        solution(p.solution),
        nNodes(p.nNodes),
        nEdges(p.nEdges),
        nSolution(p.nSolution),
        matrix(p.matrix),
        potentials(p.potentials) { }

/*
* Assignment Operator
*/
Problem& Problem::operator=(const Problem &p) {
    solution = p.solution;
    nNodes = p.nNodes;
    nEdges = p.nEdges;
    nSolution = p.nSolution;
    matrix = p.matrix;
    potentials = p.potentials;
    return *this;
}

/**
 * Returns a Problem using a filename
 */
Problem Problem::fromFile(string filename) {
    Problem prob;

    // Open input stream from filename (throw error if not found)
    ifstream in(filename);
    if (!in) {
        cout << "File " << filename << " not found!" << endl;
        exit(EXIT_FAILURE);
    }

    // Read nNodes and solutionSize
    in >> prob.nNodes >> prob.nSolution;
    prob.nEdges = (prob.nNodes*(prob.nNodes - 1)) / 2;

    // Set size for matrix and temporal potentials;
    prob.matrix.resize((unsigned long) (prob.nNodes * prob.nNodes));
    vector<int> temp_potentials((unsigned long) prob.nNodes);

    // Read edges
    {
        for (int i=0; i < prob.nEdges; i++) {
            int from, to; double length;
            in >> from >> to >> length;
            temp_potentials[from] += length;
            temp_potentials[to] += length;
            prob.matrix[prob.matIndex(from, to)] = length;
            prob.matrix[prob.matIndex(to, from)] = length;
        }
    }

    // Fill and sort potentials
    for (int i=0; i<prob.nNodes; i++)
        prob.potentials.push_back(pair<int,double>(i,temp_potentials[i]));
    sort(prob.potentials.begin(),
         prob.potentials.end(),
         [](pair<int,double> a, pair<int,double> b){ return a.second > b.second; });

    return prob;
}

/**
 * Obtain problem initial solution
 * using the greedy strategy
 */
void Problem::initGreedy() {
    solution.initGreedy();
}

/**
 * Obtain problem initial solution
 * using a random strategy
 */
void Problem::initRandom() {
    solution.initRandom();
}

/**
 * Solve a problem using a single local search
 */
void Problem::solveByLocalSearch() {
    solution.doLocalSearch();
}

/**
 * Solve a problem using the Basic VNS by Brimberg.
 * metaheuristic
 */
void Problem::solveByVNS() {
    const int INIT_SIZE_PERTURBATION = (int) (nSolution * 0.1); // TODO CHECK
    const int MAX_ITERATIONS_VNS = 100000; // TODO CHECK
    const int MAX_TRIES_K = 10; // TODO CHECK
    int k = INIT_SIZE_PERTURBATION;  // Size for shaking

    solution.doLocalSearch();
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
        workingSolution.doLocalSearch();

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

    solution.doLocalSearch();
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
            solution.doLocalSearch();
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
 * Solve problem using genetic algorithm
 */
void Problem::solveByGenetic() {
    const long POP_SIZE = 100;
    const long N_GENERATIONS = 200;
    const double INVERSION_RATE = 0.25;

    // Generate initial population
    vector<GeneticSolution> population;
    for (auto i=0; i<POP_SIZE; i++)
        population.push_back(GeneticSolution(this));

    // Generate a number a of generations
    for (auto i=0; i<N_GENERATIONS; i++) {
        // Selection of elements for crossover
        // Roulette-wheel sampling with simple elitism
        vector<long> selection = rouletteSelection(population, (long) (population.size() * INVERSION_RATE));

        // Crossover of selected elements by inversion operator
        for (auto j : selection) {
            population[j].doInversion();
        }
    }

    // Get best solution with value
    int bestNode = 0;
    double bestValue = population[0].value;
    for (auto i=0; i<population.size(); i++) {
        if (bestValue < population[i].value) {
            bestValue = population[i].value;
            bestNode = i;
        }
    }
    solution.value = population[bestNode].value;

}

/**
 * Get edge weigth from source and dest
 */
double Problem::getEdge(long i, long j) {
    return matrix[matIndex(i,j)];
}

/**
 * Get unidimensional index based on two dimensins
 */
long Problem::matIndex(long i, long j) {
    return nNodes * i + j;
}
