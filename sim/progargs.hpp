#ifndef PROGARGS_H
#define PROGARGS_H

#include <string>

struct ParametrosBloque {
    double s_x, s_y, s_z;
    int n_x, n_y, n_z;
};

struct ProgArgs {
    int nts{};
    std::string inputfile;
    std::string outputfile;

    ProgArgs() = default;
    ProgArgs(int argc, char *argv[]);
};

#endif
