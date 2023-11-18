#include "../sim/block.hpp"
#include "../sim/grid.hpp"
#include "../sim/particle.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  ProgArgs const args(argc, argv);

  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);

  grid malla = CalcularVariables(inputfile);

  std::vector<Particle> particles = leerparticulas(inputfile, malla);

  mostrardatos();
  // Inicio de simulación
  IniciarSimulacion(args, outputfile, malla, particles);

  return 0;
}