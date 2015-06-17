/**
 * Helper functions declarations
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */

#ifndef CI5652_MDP_HELPERS_TCC
#define CI5652_MDP_HELPERS_TCC

#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

/**
 * Pops a random element from a vector
 */
template <typename T>
T pop_random(vector<T>&v) {
    // Get the random index
    unsigned long random = rand() % v.size();
    T ans = v[random];
    swap(v[random], v.back());
    v.pop_back();
    return ans;
}

/**
 * Returns a vector with N indices of a given vector
 */
template <typename T>
vector<int> chooseNRandomFromVector(const vector<T> &v, int n) {
    vector<int> ncIndices, out;
    // Start with a vector of all notChosen possible indices
    for (auto i=0; i<v.size(); i++)
        ncIndices.push_back(i);

    // Choose n from them
    for (auto i=0; i<n; i++)
        out.push_back(pop_random(ncIndices));

    return out;
}

#endif //CI5652_MDP_HELPERS_TCC
