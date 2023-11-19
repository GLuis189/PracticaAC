#ifndef PROGARGS_H
#define PROGARGS_H

#include <string>

struct ProgArgs {
    int nts;
    std::string inputfile;
    std::string outputfile;

    ProgArgs() {};
    ProgArgs(int argc, char *argv[]);
};

#endif
