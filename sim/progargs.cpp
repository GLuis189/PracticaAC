#include <iostream>
#include <fstream>
#include <cstdlib>
#include "progargs.hpp"

ProgArgs::ProgArgs(int argc, char *argv[]) {
    if(argc != 4) {
        std::cerr << "Error: Invalid number of arguments: " << (argc - 1) << ".\n";
        exit(-1);
    }

    nts = atoi(argv[1]);
    if(nts == 0 && argv[1][0] != '0') {
        std::cerr << "Error: time steps must be numeric.\n";
        exit(-1);
    }

    if(nts < 0) {
        std::cerr << "Error: Invalid number of time steps.\n";
        exit(-2);
    }

    std::ifstream test_input(argv[2], std::ios::binary);
    if(!test_input.is_open()) {
        std::cerr << "Error: Cannot open " << argv[2] << " for reading.\n";
        exit(-3);
    }
    test_input.close();
    inputfile = argv[2];

    std::ofstream test_output(argv[3], std::ios::binary);
    if(!test_output.is_open()) {
        std::cerr << "Error: Cannot open " << argv[3] << " for writing.\n";
        exit(-4);
    }
    test_output.close();
    outputfile = argv[3];
}
