#ifndef SIMULACION_H
#define SIMULACION_H

#include <iostream>
#include "../sim/progargs.hpp"

grid CalcularVariables(std::ifstream& inputfile);

std::vector<Particle> leerparticulas(std::ifstream& inputfile, grid & malla);

void IniciarSimulacion(ProgArgs args, std::ofstream& outputfile, grid & malla, std::vector<Particle> & particles);

void mostrardatos();


#endif