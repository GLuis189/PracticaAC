
//
// Created by luis on 30/10/23.
//

#include <iostream>
#include "grid.hpp"
#include <cmath>
#include "../sim/variablesglobales.hpp"


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
        Block newBlock;
        newBlock.b_i = var_a;
        newBlock.b_j = var_b;
        newBlock.b_k = var_c;
        blocks[block_key] = newBlock;
        /*if(var_a ==0||var_a ==(n_x-1)){colisionesCx.push_back(block_key);}
        if(var_b ==0|| var_b ==(n_y-1)){colisionesCy.push_back(block_key);}
        if(var_c ==0|| var_c ==(n_z-1)){colisionesCz.push_back(block_key);}*/
        calcularBloquesAdyacentes(block_key, var_a, var_b, var_c);
      }
    }
  }
}
int grid::generarClaveBloque(int var_a, int var_b, int var_c) {
  return var_a*10000 + var_b*100 + var_c;
}

bool grid::esValido(int indice, int max) {
  return indice >= 0 && indice < max;
}

void grid::calcularBloquesAdyacentes(const int block_key, int vara, int varb, int varc) {
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        for (int k = -1; k < 2; k++) {
          if (esValido(vara+i, n_x) && esValido(varb+j, n_y) && esValido(varc+k, n_z)){
            int const block_key2 = generarClaveBloque(vara+i, varb+j, varc+k);
            blocks[block_key].bloques_ady.emplace_back(block_key2);
          }
        }
      }
    }
}

void grid::CambiarBloque(int ide, int b_i, int b_j, int b_k, int i_anterior, int j_anterior, int k_anterior) {
  int const block_key = generarClaveBloque(i_anterior, j_anterior, k_anterior);
  blocks[block_key].removeParticle(ide);
  int const block_key2 = generarClaveBloque(b_i, b_j, b_k);
  blocks[block_key2].addParticle(ide);
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
    particle.densidad = (particle.densidad + pow(suavizado, 6)) * 315 * masa/ (64 * M_PI * pow(suavizado, 9));
  }
}

void grid::calcularAceleraciones(std::vector<Particle> & particles, double pi_sua_6, double masa, double suavizado){
  for (Particle& particle : particles) {
    for(int const ide_particula: particle.particulas_adyacentes){
      Particle& particula = particles[ide_particula];
      double const distancia = particle.CalcularDistancia(particula);
      double const dist_ij = std::sqrt(std::max(distancia, 1e-12));
      particle.VariacionAcelaracion(particula, suavizado, pi_sua_6, dist_ij, masa);
    }
    particle.particulas_adyacentes.clear();
  }
}

  /*void grid:: ColisionesEjeX_1(std::vector<Particle> & particles) {
    for (int  const& bloque : colisionesCx) {
      std::vector<int>  const& particulas_0x = blocks[bloque].particles;
      for (int const id_p : particulas_0x) {
        Particle & particula = particles[id_p];
        if (particula.p_i == 0){
          particula.ColisionesEjeX0_1();
        }
        else{
          particula.ColisionesEjeXnx_1();
        }
      }
    }
  }

  void grid::ColisionesEjeY_1(std::vector<Particle> & particles) {
    for (int const& bloque: colisionesCy){
      std::vector<int> const& particulas_0y = blocks[bloque].particles;
      for (int const id_p : particulas_0y) {
        Particle & particula = particles[id_p];
        if (particula.p_j == 0){
          particula.ColisionesEjeY0_1();
        }
        else{
          particula.ColisionesEjeYny_1();
        }
      }
    }
  }*/

  /*void grid::ColisionesEjeZ_1(std::vector<Particle> & particles) {
    for (int const& bloque: colisionesCz){
      std::vector<int> const& particulas_0z = blocks[bloque].particles;
      for (int const id_p : particulas_0z) {
        Particle & particula = particles[id_p];
        if (particula.p_k == 0){
          particula.ColisionesEjeZ0_1();
        }
        else {
          particula.ColisionesEjeZnz_1();
        }
      }
    }
  }*/

  /*void grid::ColisionesEjeX_2(std::vector<Particle> & particles) {
    for (int const& bloque: colisionesCx) {
      std::vector<int> const& particulas_0x = blocks[bloque].particles;
      for (int const id_p : particulas_0x) {
        Particle & particula = particles[id_p];
        particula.ColisionesEjeX_2();
      }
    }
  }
  void grid::ColisionesEjeY_2(std::vector<Particle> & particles) {
    for (int const& bloque: colisionesCy) {
      std::vector<int> const& particulas_0y = blocks[bloque].particles;
      for (int const id_p : particulas_0y) {
        Particle & particula = particles[id_p];
        particula.ColisionesEjeY_2();
      }
    }
  }
  void grid::ColisionesEjeZ_2(std::vector<Particle> & particles) {
    for (int const& bloque: colisionesCz) {
      std::vector<int> const& particulas_0z = blocks[bloque].particles;
      for (int const id_p : particulas_0z) {
        Particle & particula = particles[id_p];
        particula.ColisionesEjeZ_2();
      }
    }
  }*/