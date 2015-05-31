//
// Created by shadowmanu on 28/05/15.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_set>

#include "Problem.h"

using namespace std;

Solution::Solution(Problem *p) : problem(p) {}

void Solution::getInitial() {
    nSolution = problem->nSolution;
    initialSolution();
}

void Solution::solLocalSearch() {
    // TODO COPY LOCAL SEARCH
    const int ALLOWED_REPETITIONS = (int) ((problem->nNodes - nSolution) * 0.05);

    double currentDistance = calcSolutionValue();

    // For each node from right to left
    for (int i=nSolution-1; i>=0; i--) {
        int original = elements[i];
        unordered_set<int>tested;
        bool modified = false;
        int newNode, lastNode;

        // Search best replacing node from notChosen ones
        // with a limited number of
        for (int j=0;j<ALLOWED_REPETITIONS && tested.size()<notChosen.size();) {

            // Choose an untested node from those not in solution
            do {
                newNode = (int) (rand() % notChosen.size());
            } while (tested.find(newNode)!=tested.end());
            tested.insert(newNode);

            // Replace node
            elements[i] = notChosen[newNode];
            double newDistance = calcSolutionValue();

            // Check if its an improvement
            if (currentDistance < newDistance) {
                currentDistance = newDistance;
                lastNode = newNode;
                modified = true;
                j=0;
            // Else, keep searching
            } else {
                j++;
            }
        }
        // If modified, send best to solution and original to not chosen
        if (modified) {
            elements[i] = notChosen[lastNode];
            notChosen[lastNode] = original;
        }
        // Else, put original back in its place
        else elements[i] = original;
    }
}

double Solution::calcSolutionValue() {
    double total = 0;

    for (int i=0; i<nSolution; i++){
        for (int j=i+1; j<nSolution; j++){
            total += problem->getEdge(elements[i],elements[j]);
        }
    }

    return total;
}

bool potentialComp(pair<int,double> a, pair<int,double> b) {
    return a.second > b.second;
}

void Solution::initialSolution() {
    // Get nodes with potentials as pairs
    vector<pair<int,double>> ord_potentials;
    for (int i=0; i<problem->nNodes; i++) {
        ord_potentials.push_back(pair<int,double>(i,problem->potentials[i]));
    }

    // Sort them by potentials
    sort(ord_potentials.begin(),ord_potentials.end(),potentialComp);

    // Choose first nSolution as initial solution
    for (int i=0; i<nSolution; i++) {
        elements.push_back(ord_potentials[i].first);
    }

    // Classify others as not chosen
    for (int i=nSolution; i<problem->nNodes; i++) {
        notChosen.push_back(ord_potentials[i].first);
    }
}

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

    // Set sizes for matrix, potentials and initialize solution
    matrix.resize((unsigned long) (nNodes * nNodes));
    potentials.resize((unsigned long) nNodes);

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

void Problem::getInitial() {
    solution.getInitial();
}

int Problem::matIndex(int i, int j) {
    return nNodes * i + j;
}

double Problem::getEdge(int i, int j) {
    return matrix[matIndex(i,j)];
}

void Problem::solve() {
    // TODO METAHEURISTICS
}

void Problem::solLocalSearch() {
    solution.solLocalSearch();
}
