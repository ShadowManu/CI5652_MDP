/**
 * Helper functions implementations
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */

#include <algorithm>
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