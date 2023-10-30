#include <iostream>
#include <fstream>
#include <cstdlib>
#include "progargs.hpp"

int main(int argc, char *argv[]) {
    ProgArgs args(argc, argv);

    std::ifstream inputfile(args.inputfile);
    std::ofstream outputfile(args.outputfile);

    // Aquí va el código de la simulación

    return 0;
}
