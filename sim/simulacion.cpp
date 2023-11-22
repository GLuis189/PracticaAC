#include "../sim/grid.hpp"
#include "../sim/progargs.hpp"
#include "../sim/variablesglobales.hpp"
#include "../sim/simulacion.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <array>


double suavizado_2; //NOLINT
double ppm;         //NOLINT
int numparticulas;  //NOLINT
double masa;        //NOLINT
double suavizado;   //NOLINT
double pi_sua_6;    //NOLINT


grid calcularMalla(std::ifstream& inputfile) {
  ppm           = read_binary_value<float>(inputfile);
  numparticulas = read_binary_value<int>(inputfile);

  if (numparticulas <= 0) {
    std::cerr << "Error: Invalid number of particles: 0.\n";
    exit(errorconst);
  }
  masa      = densidad / (ppm * ppm * ppm);
  suavizado = radio / ppm;
  pi_sua_6 = M_PI * suavizado * suavizado * suavizado * suavizado * suavizado * suavizado;
  suavizado_2 = suavizado * suavizado;

  grid malla(suavizado);
  return malla;
}

std::vector<Particle> leerParticulas(std::ifstream& inputfile, grid & malla) {
  std::vector<Particle> particles;
  particles.reserve(numparticulas);
  int contar_particulas = 0;
  float p_x, p_y, p_z, hvx, hvy, hvz, v_x, v_y, v_z;  //NOLINT
  while(read_binary_values(inputfile,  p_x, p_y, p_z, hvx, hvy, hvz, v_x, v_y, v_z)) {
    Particle particle;
    particle.ide = contar_particulas, particle.densidad = 0;
    particle.posicion.c_x = p_x, particle.posicion.c_y = p_y, particle.posicion.c_z = p_z;
    particle.hvelocidad.c_x = hvx, particle.hvelocidad.c_y = hvy, particle.hvelocidad.c_z = hvz;
    particle.velocidad.c_x = v_x, particle.velocidad.c_y = v_y, particle.velocidad.c_z = v_z;
    particle.aceleracion.c_y = g_y, particle.aceleracion.c_x = 0, particle.aceleracion.c_z = 0;
    malla.calcularBloqueInicial(particle);
    int const block_key = grid::generarClaveBloque(particle.p_i, particle.p_j, particle.p_k);
    malla.blocks[block_key].addParticle(particle.ide);
    particles.emplace_back(particle);
    ++contar_particulas;
  }
  inputfile.close();
  if (numparticulas != contar_particulas) {
    std::cerr << "Error: Number of particles mismatch. Header:" << numparticulas
              << ", Found:" << contar_particulas << ".\n";
    exit(errorconst);
  }
 return particles;
}

void reposicionar(grid & malla, std::vector<Particle> & particles) {
  for (Particle& particle : particles) {
    particle.Inicializar();
    int const i_anterior = particle.p_i;
    int const j_anterior = particle.p_j;
    int const k_anterior = particle.p_k;
    malla.calcularBloque(particle);
    if (i_anterior != particle.p_i || j_anterior != particle.p_j || k_anterior != particle.p_k) {
      malla.CambiarBloque(particle, i_anterior, j_anterior, k_anterior);
    }
  }
}

void ResultadosBinarios(std::vector<Particle>& particulas, std::ofstream& outputfile) {
  auto ppm2 = static_cast<float>(ppm);

  outputfile.write(reinterpret_cast<const char*>(&ppm2), sizeof(ppm2));     //NOLINT
  outputfile.write(reinterpret_cast<const char*>(&numparticulas), sizeof(numparticulas));     //NOLINT

  for (const auto& particula : particulas) {
    std::array<float, n_9> values = {
      static_cast<float>(particula.posicion.c_x),
      static_cast<float>(particula.posicion.c_y),
      static_cast<float>(particula.posicion.c_z),
      static_cast<float>(particula.hvelocidad.c_x),
      static_cast<float>(particula.hvelocidad.c_y),
      static_cast<float>(particula.hvelocidad.c_z),
      static_cast<float>(particula.velocidad.c_x),
      static_cast<float>(particula.velocidad.c_y),
      static_cast<float>(particula.velocidad.c_z)
    };

    outputfile.write(reinterpret_cast<const char*>(values.data()), sizeof(values));     //NOLINT
  }

  outputfile.close();
}

void IniciarSimulacion(const ProgArgs& args, std::ofstream& outputfile, grid & malla, std::vector<Particle>& particles){
  for (int time = 0; time < args.nts; time++) {
    if(time>0) {
     reposicionar(malla, particles);
    }
    malla.calcularDensidades(particles, masa, suavizado, suavizado_2);
    grid::calcularAceleraciones(particles, pi_sua_6, masa, suavizado);
    for (Particle& particle : particles){
      particle.MoverParticulas(malla.n_x, malla.n_y, malla.n_z);
    }
  }

  // Mostrar los datos de las partículas
  ResultadosBinarios(particles, outputfile);
}


void mostrarDatos(grid & malla){
  // Mostrar los datos
  std::cout << "Number of particles: " << numparticulas << "\n";
  std::cout << "Particles per meter: " << ppm << "\n";
  std::cout << "Smoothing length: " << suavizado << "\n";
  std::cout << "Particle mass: " << masa << "\n";
  std::cout << "Grid size: " << malla.n_x << " x " << malla.n_y << " x " << malla.n_z << "\n";
  std::cout << "Number of blocks: " << malla.n_x * malla.n_y * malla.n_z << "\n";
  std::cout << "Block size: " << malla.s_x << " x " << malla.s_y << " x " << malla.s_z << "\n";
}

