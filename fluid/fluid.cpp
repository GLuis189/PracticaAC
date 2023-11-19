#include "../sim/block.hpp"
#include "../sim/grid.hpp"
#include "../sim/progargs.hpp"
#include "../sim/variablesglobales.hpp"
#include "../sim/simulacion.hpp"

#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  ProgArgs const args(argc, argv);

  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);

  grid malla = calcularMalla(inputfile);

  std::vector<Particle> particles = leerParticulas(inputfile, malla);

  mostrarDatos();
  // Inicio de simulación
  IniciarSimulacion(args, outputfile, malla, particles);

  return 0;
}