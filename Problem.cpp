//
// Created by shadowmanu on 28/05/15.
//

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Problem.h"

using namespace std;

Solution::Solution(Problem *p) : problem(p) {}

void Solution::getInitial() {
    nSolution = problem->nSolution;
    initialSolution();
}

void Solution::solLocalSearch() {
    // TODO COPY LOCAL SEARCH
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
