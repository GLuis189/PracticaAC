//
// Created by luis on 30/10/23.
//

#include <iostream>
#include "grid.hpp"
#include <cmath>
#include "../sim/variablesglobales.hpp"
#include <algorithm>


grid::grid(double suavizado)
  : n_x(static_cast<int>((bmax_x - bmin_x) / suavizado)),
    n_y(static_cast<int>((bmax_y - bmin_y) / suavizado)),
    n_z(static_cast<int>((bmax_z - bmin_z) / suavizado)),
    s_x((bmax_x - bmin_x) / n_x),
    s_y((bmax_y - bmin_y) / n_y),
    s_z((bmax_z - bmin_z) / n_z)
{
  // Generar bloques para toda la malla
  for (int var_a = 0; var_a < n_x; ++var_a) {
    for (int var_b = 0; var_b < n_y; ++var_b) {
      for (int var_c = 0; var_c < n_z; ++var_c) {
        int const block_key = generarClaveBloque(var_a, var_b, var_c);
        Block const newBlock;
        blocks[block_key] = newBlock;
        calcularBloquesAdyacentes(block_key, var_a, var_b, var_c);
      }
    }
  }
}
int grid::generarClaveBloque(int var_a, int var_b, int var_c) {
  return var_a*dmil + var_b*cien + var_c;
}

bool grid::esValido(int indice, int max) {
  return indice >= 0 && indice < max;
}

void grid::calcularBloquesAdyacentes(const int block_key, int var_a, int var_b, int var_c) {
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        for (int k = -1; k < 2; k++) {
          if (esValido(var_a+i, n_x) && esValido(var_b+j, n_y) && esValido(var_c+k, n_z)){
            int const block_key2 = generarClaveBloque(var_a+i, var_b+j, var_c+k);
            blocks[block_key].bloques_ady.emplace_back(block_key2);
          }
        }
      }
    }
}

void grid::calcularBloque(Particle & particle) const {
    particle.p_i = std::clamp(static_cast<int>((particle.posicion.c_x - bmin_x) / s_x), 0, n_x - 1);
    particle.p_j = std::clamp(static_cast<int>((particle.posicion.c_y - bmin_y) / s_y), 0, n_y - 1);
    particle.p_k = std::clamp(static_cast<int>((particle.posicion.c_z - bmin_z) / s_z), 0, n_z - 1);
}
void grid:: calcularBloqueInicial(Particle & particle) const{
    particle.p_i = static_cast<int>((particle.posicion.c_x-bmin_x)/ s_x);
    if (particle.p_i> n_x -1){
      particle.p_i = n_x -1;
    }
    particle.p_j = static_cast<int>((particle.posicion.c_y-bmin_y)/ s_y);
    if (particle.p_j>n_y -1){
      particle.p_j = n_y-1;
    }
    particle.p_k = static_cast<int>((particle.posicion.c_z-bmin_z)/ s_z);
    if (particle.p_k>n_z -1){
      particle.p_k = n_z -1;
    }
}

void grid::CambiarBloque(Particle & particle, int i_anterior, int j_anterior, int k_anterior) {
  int const block_key = generarClaveBloque(i_anterior, j_anterior, k_anterior);
  blocks[block_key].removeParticle(particle.ide);
  int const block_key2 = generarClaveBloque(particle.p_i, particle.p_j, particle.p_k);
  blocks[block_key2].addParticle(particle.ide);
}

void grid::calcularDensidades(std::vector<Particle> & particles, double masa, double suavizado, double suavizado_2) {
  for (Particle& particle : particles) {
    int const block_key2 = generarClaveBloque(particle.p_i, particle.p_j, particle.p_k);
    std::vector<int> const bloquesAdy = blocks[block_key2].bloques_ady;
    for (int const& bloque : bloquesAdy) {
      std::vector<int> const& particulas_ady = blocks[bloque].particles;
      for (int const id_p : particulas_ady) {
        Particle & particula = particles[id_p];
        if (particula > particle) { particle.VariacionDensidad(particula, suavizado_2); }
      }
    }
    particle.densidad = (particle.densidad + pow(suavizado, n_6)) * n_315 * masa/ (n_64 * M_PI * pow(suavizado, n_9));
  }
}

void grid::calcularAceleraciones(std::vector<Particle> & particles, double pi_sua_6, double masa, double suavizado){
  for (Particle& particle : particles) {
    for(int const ide_particula: particle.particulas_adyacentes){
      Particle& particula = particles[ide_particula];
      particle.VariacionAcelaracion(particula, suavizado, pi_sua_6, masa);
    }
    particle.particulas_adyacentes.clear();
  }
}