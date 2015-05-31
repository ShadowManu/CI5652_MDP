//
// Created by shadowmanu on 28/05/15.
//

#include <string>
#include "main.h"
#include "Problem.h"



int main(int argc, char *argv[]) {
    string filename;
    // If file argument, read from it (else, cin)
    if (argc >= 2) {
        filename = argv[1];
    }

    // Create problem
    Problem prob(filename);
    return 0;
}
