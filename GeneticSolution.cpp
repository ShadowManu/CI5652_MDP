/**
 * Genetic Solution class implementation
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */

#include "Types.h"
#include "Helpers.h"

/**
 * Default constructor
 */
GeneticSolution::GeneticSolution() : nSolution(0), value(0), bitmap(), problem(nullptr) { }

/**
 * Copy constructor using problem (initializes itself)
 * No initRandom() call required
 */
GeneticSolution::GeneticSolution(Problem *p) :
        nSolution(p->nSolution),
        value(0),
        bitmap((unsigned long) p->nNodes, false),
        problem(p) {

    // Generate options to activate
    vector<long> options = vectorRange(bitmap.size());

    // Choose nSolution elements
    for (auto i=0; i<nSolution; i++) {
        auto index = pop_random(options);
        bitmap[index] = true;
    }

    // Calculate initial value
    for (auto i=0; i<bitmap.size(); i++) {
        if (bitmap[i]) {
            for (auto j=i+1; j<bitmap.size(); j++) {
                if (bitmap[j]) {
                    value += problem->getEdge(i,j);
                }
            }
        }
    }
}

/**
 * Copy constructor using a genetic solution
 */
GeneticSolution::GeneticSolution(const GeneticSolution &gs) :
        nSolution(gs.nSolution),
        value(gs.value),
        bitmap(gs.bitmap),
        problem(gs.problem) { }

/**
 * Assignment Operator
 */
GeneticSolution& GeneticSolution::operator=(const GeneticSolution &gs) {
    nSolution = gs.nSolution;
    value = gs.value;
    bitmap = gs.bitmap;
    problem = gs.problem;
}

/**
 * Makes a simple inversion in the genetic solution
 */
void GeneticSolution::doInversion() {
    // Choose range points for inversion
    long start = rand() % (bitmap.size() - 1); // Unable to choose last
    long end = (rand() % (bitmap.size() - 1)) + 1; // Unable to choose first
    long topmid = (start + end + 1) / 2;

    // From start to end, toggle inverts
    for (auto i=start; i<topmid; i++) {
        if (bitmap[i] != bitmap[end+start-i]) {
            toggle(i);
            toggle(end+start-i);
        }
    }
}

/**
 * Activates a bit updating value
 */
void GeneticSolution::toggle(long i) {
    // Switch bit
    bitmap[i] = !bitmap[i];

    // Get Direction
    long direction = bitmap[i] ? 1 : -1;

    // Update value
    for (auto j=0; j<bitmap.size(); j++) {
        if (bitmap[j] && j!=i)
            value += problem->getEdge(i,j) * direction;
    }
}