/**
 * Main implementation of MDP solver
 *
 * Authors:
 *      Manuel Pacheco | 10-10524
 *      Cristian Medina | 10-10445
 */
#include <string>
#include <istream>
#include <iostream>
#include <ctime>
#include <algorithm>

#include "main.h"
#include "Problem.h"

using namespace std;

double avg ( vector<double>& v )
{
    double return_value = 0.0;
    int n = v.size();

    for ( int i=0; i < n; i++)
    {
        return_value += v[i];
    }

    return ( return_value / n);
}

int main(int argc, char *argv[]) {
    // Startup random seed
    srand((unsigned int) time(NULL));

    vector<double> values;
    vector<double> times;

    cout << "VNS:" << endl;
    // 15 times
    for (int i = 0; i<15; i++) {
        clock_t begin = clock();

        // Assign file argument if provided
        string inName;
        if (argc >= 2) inName = argv[1];

        // Create problem
        Problem prob(inName);

        // Get initial solution
        prob.getInitial();
        if (i == 1) cout << prob.solution.value << "\t\t" << endl;
        // Solve by Tabu
        prob.solveByVNS();

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        cout << prob.solution.value << "\t\t" << elapsed_secs << endl;
        values.push_back(prob.solution.value);
        times.push_back(elapsed_secs);
    }

    cout << "VALUES | MIN: " << *min_element(values.begin(), values.end())
            << " MAX: " << *max_element(values.begin(), values.end())
            << " AVERAGE: " << avg(values) << endl;

    cout << "TIMES | MIN: " << *min_element(times.begin(), times.end())
           << " MAX: " << *max_element(times.begin(), times.end())
           << " AVERAGE: " << avg(times) << endl;

    values.clear();
    times.clear();

    cout << "TABOO SEARCH:" << endl;
    // 15 times
    for (int i = 0; i<15; i++) {
        clock_t begin = clock();

        // Assign file argument if provided
        string inName;
        if (argc >= 2) inName = argv[1];

        // Create problem
        Problem prob(inName);

        // Get initial solution
        prob.getInitial();
        // Solve by Tabu
        prob.solveByTabu();

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        cout << prob.solution.value << "\t\t" << elapsed_secs << endl;
        values.push_back(prob.solution.value);
        times.push_back(elapsed_secs);
    }

    cout << "VALUES | MIN: " << *min_element(values.begin(), values.end())
           << " MAX: " << *max_element(values.begin(), values.end())
           << " AVERAGE: " << avg(values) << endl;

    cout << "TIMES | MIN: " << *min_element(times.begin(), times.end())
           << " MAX: " << *max_element(times.begin(), times.end())
           << " AVERAGE: " << avg(times) << endl;

    values.clear();
    times.clear();

    cout << "LOCAL SEARCH:" << endl;
    // 15 times
    for (int i = 0; i<15; i++) {
        clock_t begin = clock();

        // Assign file argument if provided
        string inName;
        if (argc >= 2) inName = argv[1];

        // Create problem
        Problem prob(inName);

        // Get initial solution
        prob.getInitial();

        // Local search once
        prob.solLocalSearch();

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        cout << prob.solution.value << "\t\t" << elapsed_secs << endl;
        values.push_back(prob.solution.value);
        times.push_back(elapsed_secs);
    }

    cout << "VALUES | MIN: " << *min_element(values.begin(), values.end())
           << " MAX: " << *max_element(values.begin(), values.end())
           << " AVERAGE: " << avg(values) << endl;

    cout << "TIMES | MIN: " << *min_element(times.begin(), times.end())
           << " MAX: " << *max_element(times.begin(), times.end())
           << " AVERAGE: " << avg(times) << endl;

    values.clear();
    times.clear();

    return 0;
}
