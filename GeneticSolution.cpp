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
    return *this;
}

/**
 * Returns a vector of indices of the chosen solutions
 * of a population using a roulette-wheel sampling
 * technique (with simple elitism)
 */
vector<long> GeneticSolution::rouletteSelection(const vector<GeneticSolution> &population, long n) {
    double groupFitness = 0;
    vector<double> wheelRanges(1,0);

    // Find the best objective function and index (for simple elitism)
    double bestValue = population[0].value;
    long bestIndex = 0;
    for (auto i=1; i<population.size(); i++) {
        if (bestValue > population[i].value) {
            bestValue = population[i].value;
            bestIndex = i;
        }
    }

    // Create the wheel ranges
    for (auto i=0; i<population.size(); i++) {
        // Skip best solution
        if (i == bestIndex) continue;

        // Obtain fitness value
        double fitness = population[i].value / bestValue; // #!

        // Add fitness to group (using objective function value)
        groupFitness += fitness;

        // Add range
        wheelRanges.push_back(groupFitness);
    }

    // Select the n elements from the population
    vector<long> output;
    double top = wheelRanges.back();
    while (output.size() != n) {
        // Generate random number in wheel range
        double random = fmod(rand() * 10, top);

        // Transform random in wheel index using binary search
        long left = 0, right = wheelRanges.size()-1;
        while (left < right-1) {
            long mid = (left + right) / 2;
            // Match case
            if (wheelRanges[mid] <= random &&
                random < wheelRanges[mid+1]) {
                left = mid;
                break;
                // Left case
            } else if (random < wheelRanges[mid]) {
                right = mid;
                // Right case
            } else if (wheelRanges[mid+1] <= random) {
                left = mid+1;
            }
        }

        // Add to chosen and output
        output.push_back(left);
    }

    // Return selection
    return output;
}

/**
 * Makes a simple inversion in the genetic solution
 */
void GeneticSolution::geneticCrossover (
        GeneticSolution &p1,
        GeneticSolution &p2) {

    GeneticSolution o1(p1);
    GeneticSolution o2(p2);

    long size = p1.bitmap.size();
    bool alternator = true;

    // Fix differences between offsprings
    for (auto i=0; i<size; i++) {
        if (o1.bitmap[i] != o2.bitmap[i]) {
            // First one receives it
            if (alternator) {
                if (!o1.bitmap[i]) o1.toggle(i);
                if (o2.bitmap[i]) o2.toggle(i);
            // Second one receives it
            } else {
                if (o1.bitmap[i]) o1.toggle(i);
                if (!o2.bitmap[i]) o2.toggle(i);
            }
            alternator = !alternator;
        }
    }

    // Offsprings replace parents if they improve them
    if (p1.value < o1.value) p1 = o1;
    if (p2.value < o2.value) p2 = o2;
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

/**
 * Mutates the bit of a solution
 */
void GeneticSolution::mutate() {
    const long trueJumps = rand() % nSolution + 1;
    const long falseJumps = rand() % (problem->nNodes - nSolution) + 1;
    long jumps, i;

    // Remove a random bit
    for (i=0, jumps=0; i <bitmap.size(); ++i) {
        if (bitmap[i]) jumps++;
        if (jumps==trueJumps) break;
    }
    this->toggle(i);

    // Add a random bit
    for (i=0, jumps=0; i<bitmap.size(); ++i) {
        if (!bitmap[i]) jumps++;
        if (jumps==falseJumps) break;
    }
    this->toggle(i);
}