#include "../sim/block.hpp"
#include "../sim/grid.hpp"
#include "../sim/progargs.hpp"
#include "../sim/variablesglobales.hpp"
#include "../sim/simulacion.hpp"

#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<std::string> const argv_vector(argv, argv + argc);
  ProgArgs const args(argc, argv_vector);

  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);

  grid malla = calcularMalla(inputfile);

  std::vector<Particle> particles = leerParticulas(inputfile, malla);

  mostrarDatos(malla);
  // Inicio de simulación
  IniciarSimulacion(args, outputfile, malla, particles);

  return 0;
}