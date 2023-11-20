#ifndef SIMULACION_H
#define SIMULACION_H

#include <iostream>
#include "../sim/progargs.hpp"

grid calcularMalla(std::ifstream& inputfile);

std::vector<Particle> leerParticulas(std::ifstream& inputfile, grid & malla);
void reposicionar(grid & malla, std::vector<Particle> & particles);

void IniciarSimulacion(const ProgArgs& args, std::ofstream& outputfile, grid & malla, std::vector<Particle> & particles);

void mostrarDatos();
void ResultadosBinarios(std::vector<Particle> & particulas,std::ofstream& outputfile);

#endif