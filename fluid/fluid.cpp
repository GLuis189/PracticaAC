#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../sim/progargs.hpp"

int main(int argc, char *argv[]) {
    ProgArgs args(argc, argv);

    std::ifstream inputfile(args.inputfile, std::ios::binary);
    std::ofstream outputfile(args.outputfile, std::ios::binary);

    // Aquí va el código de la simulación

    return 0;
}
