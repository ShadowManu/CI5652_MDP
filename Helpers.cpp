/**
 * Helper functions implementations
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */

#include <algorithm>
#include <unordered_set>

#include "Helpers.h"

using namespace std;

/**
 * Returns a vector containing longs from 0 to n-1
 */
vector<long> vectorRange(long n) {
    vector<long> out;
    for (auto i=0; i<n; i++) {
        out.push_back(i);
    }
    return out;
}

/**
 * Returns a vector of indices of the chosen solutions
 * of a population using a roulette-wheel sampling
 * technique (with simple elitism)
 */
vector<long> rouletteSelection(const vector<GeneticSolution> &population, long n) {
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

    // Select the n different elements from the population
    unordered_set<long> chosen;
    vector<long> output;
    double top = wheelRanges.back();
    while (chosen.size() != n) {
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

        // If it is chosen, skip
        if (chosen.find(left) != chosen.end()) continue;

        // Add to chosen and output
        chosen.insert(left);
        output.push_back(left);
    }

    // Return selection
   return output;

}