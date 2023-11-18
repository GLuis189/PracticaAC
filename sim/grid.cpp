
//
// Created by luis on 30/10/23.
//

#include <iostream>
#include "grid.hpp"
#include <cmath>
#include "../sim/variablesglobales.hpp"


grid::grid(int nx, int ny, int nz) {
  // Generar bloques para toda la malla
  for (int var_a = 0; var_a < nx; ++var_a) {
    for (int var_b = 0; var_b < ny; ++var_b) {
      for (int var_c = 0; var_c < nz; ++var_c) {
        std::string const block_key = generarClaveBloque(var_a, var_b, var_c);
        Block newBlock;
        newBlock.i = var_a;
        newBlock.j = var_b;
        newBlock.k = var_c;
        blocks[block_key] = newBlock;
        if(var_a ==0||var_a ==(nx-1)){colisionesCx.push_back(block_key);}
        if(var_b ==0|| var_b ==(ny-1)){colisionesCy.push_back(block_key);}
        if(var_c ==0|| var_c ==(nz-1)){colisionesCz.push_back(block_key);}
        calcularBloquesAdyacentes(block_key, var_a, var_b, var_c, nx, ny, nz);
      }
    }
  }
}

std::string grid::generarClaveBloque(int a, int b, int c) {
  return std::to_string(a) + "_" + std::to_string(b) + "_" + std::to_string(c);
}

void grid::calcularBloquesAdyacentes(const std::string& block_key, int a, int b, int c, int nx, int ny, int nz) {
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        for (int k = -1; k < 2; k++) {
          if (a + i >= 0 && a + i < nx && b + j >= 0 && b + j < ny && c + k >= 0 && c + k < nz) {
            std::string const block_key2 = generarClaveBloque(a+i, b+j, c+k);
            blocks[block_key].bloques_ady.push_back(block_key2);
          }
        }
      }
    }
}

void grid::CambiarBloque(int id, int i, int j, int k, int i_anterior, int j_anterior, int k_anterior) {
  std::string const block_key = generarClaveBloque(i_anterior, j_anterior, k_anterior);
  blocks[block_key].removeParticle(id);
  std::string const block_key2 = generarClaveBloque(i, j, k);
  blocks[block_key2].addParticle(id);
}

void grid::calcularDensidades(std::vector<Particle> & particles, int numparticulas, double masa, double suavizado, double suavizado_2) {
  for (int part = 0; part < numparticulas; part++) {
    Particle & particle = particles[part];
    std::string const block_key2 = generarClaveBloque(particle.i, particle.j, particle.k);
    std::vector<std::string> const bloquesAdy = blocks[block_key2].bloques_ady;
    for (std::string const& bloque : bloquesAdy) {
      std::vector<int> const& particulas_ady = blocks[bloque].particles;
      for (int const id_p : particulas_ady) {
        Particle & particula = particles[id_p];
        if (particula > particle) { particle.VariacionDensidad(particula, suavizado_2); }
      }
    }
    particle.densidad = (particle.densidad + pow(suavizado, 6)) * 315 * masa/ (64 * M_PI * pow(suavizado, 9));
  }
}

void grid::calcularAceleraciones(std::vector<Particle> & particles, int numparticulas, double pi_sua_6, double masa, double suavizado){
  for (int part = 0; part < numparticulas; part++) {
    Particle & particle          = particles[part];
    std::string const block_key2 = generarClaveBloque(particle.i, particle.j, particle.k);
    std::vector<std::string> const bloquesAdy = blocks[block_key2].bloques_ady;
    for (std::string const & bloque : bloquesAdy) {
      std::vector<int> const & particulas_ady = blocks[bloque].particles;
      for (int const id_p : particulas_ady) {
        Particle & particula = particles[id_p];
        if (particula > particle) {
            double distancia = particle.CalcularDistancia(particula);
            if (distancia < suavizado * suavizado) {

              double const dist_ij = std::sqrt(std::max(distancia, 1e-12));
              particle.VariacionAcelaracionX(particula, suavizado, pi_sua_6, dist_ij, masa);
              particle.VariacionAcelaracionY(particula, suavizado, pi_sua_6, dist_ij, masa);
              particle.VariacionAcelaracionZ(particula, suavizado, pi_sua_6, dist_ij, masa);
            }
        }
      }
    }
  }
}

void grid:: ColisionesEjeX_1(std::vector<Particle> & particles) {
  for (std::string  const& bloque : colisionesCx) {
    std::vector<int>  const& particulas_0x = blocks[bloque].particles;
    for (int const id_p : particulas_0x) {
      Particle & particula = particles[id_p];
      if (particula.i == 0){
        particula.ColisionesEjeX0_1();
      }
      else{
        particula.ColisionesEjeXnx_1();
      }
    }
  }
}

void grid::ColisionesEjeY_1(std::vector<Particle> & particles) {
  for (std::string const& bloque: colisionesCy){
    std::vector<int> const& particulas_0y = blocks[bloque].particles;
    for (int const id_p : particulas_0y) {
      Particle & particula = particles[id_p];
      if (particula.j == 0){
        particula.ColisionesEjeY0_1();
      }
      else{
        particula.ColisionesEjeYnx_1();
      }
    }
  }
}

void grid::ColisionesEjeZ_1(std::vector<Particle> & particles) {
  for (std::string const& bloque: colisionesCz){
    std::vector<int> const& particulas_0z = blocks[bloque].particles;
    for (int const id_p : particulas_0z) {
      Particle & particula = particles[id_p];
      particula.ColisionesEjeZ_1();
    }
  }
}

void grid::ColisionesEjeX_2(std::vector<Particle> & particles) {
  for (std::string const& bloque: colisionesCx) {
    std::vector<int>  const& particulas_0x = blocks[bloque].particles;
    for (int const id_p : particulas_0x) {
      Particle & particula = particles[id_p];
      particula.ColisionesEjeX_2();
    }
  }
}
void grid::ColisionesEjeY_2(std::vector<Particle> & particles) {
  for (std::string const& bloque: colisionesCy) {
    std::vector<int>  const& particulas_0y = blocks[bloque].particles;
    for (int const id_p : particulas_0y) {
      Particle & particula = particles[id_p];
      particula.ColisionesEjeY_2();
    }
  }
}
void grid::ColisionesEjeZ_2(std::vector<Particle> & particles) {
  for (std::string const& bloque: colisionesCz) {
    std::vector<int>  const& particulas_0z = blocks[bloque].particles;
    for (int const id_p : particulas_0z) {
      Particle & particula = particles[id_p];
      particula.ColisionesEjeZ_2();
    }
  }
}