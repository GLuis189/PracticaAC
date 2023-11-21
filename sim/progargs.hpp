#ifndef PROGARGS_H
#define PROGARGS_H

#include <string>
#include <vector>

struct ProgArgs {
    int nts{};
    std::string inputfile;
    std::string outputfile;

    ProgArgs() = default;
    ProgArgs(int argc, const std::vector<std::string>& argv);
};

#endif
