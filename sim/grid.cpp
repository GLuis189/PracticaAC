//
// Created by luis on 30/10/23.
//

#include <iostream>
#include "grid.hpp"
#include <cmath>
#include "../sim/variablesglobales.hpp"


grid::grid(int nx, int ny, int nz) {
  // Generar bloques para toda la malla
  for (int a = 0; a < nx; ++a) {
    for (int b = 0; b < ny; ++b) {
      for (int c = 0; c < nz; ++c) {
        std::string const block_key = generarClaveBloque(a, b, c);
        Block newBlock;
        newBlock.i = a;
        newBlock.j = b;
        newBlock.k = c;
        blocks[block_key] = newBlock;
        if(a==0|| a==(nx-1)){colisionesCx.push_back(block_key);}
        if(b==0|| b==(ny-1)){colisionesCy.push_back(block_key);}
        if(c==0||c==(nz-1)){colisionesCz.push_back(block_key);}
        calcularBloquesAdyacentes(block_key, a, b, c, nx, ny, nz);
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
      for (int id: particulas_ady) {
        Particle & particula = particles[id];
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
      for (int const id : particulas_ady) {
        Particle & particula = particles[id];
        if (particula > particle) {
            double distancia = particle.CalcularDistancia(particula);
            if (distancia < suavizado * suavizado) {

              double const dist_ij = std::sqrt(std::max(distancia, 1e-12));
              // double var_ax = particle.VariacionAcelaracionX(particula, suavizado, pi_sua_6, dist_ij, masa);
              //particle.VariacionAcelaracionY(particula, suavizado, pi_sua_6, dist_ij, masa);
              //particle.VariacionAcelaracionZ(particula, suavizado, pi_sua_6, dist_ij, masa);

              double const var_ax =
                  ((((particle.px - particula.px) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
                     ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
                    (particle.densidad + particula.densidad - 2 * densidad)) +
                   ((particula.vx - particle.vx) * (45 / pi_sua_6) * (vis * masa))) /
                  (particle.densidad * particula.densidad);
              particle.ax  = particle.ax + var_ax;
              particula.ax = particula.ax - var_ax;

              double const var_ay =
                  ((((particle.py - particula.py) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
                     ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
                    (particle.densidad + particula.densidad - 2 * densidad)) +
                   ((particula.vy - particle.vy) * (45 / pi_sua_6) * (vis * masa))) /
                  (particle.densidad * particula.densidad);
              particle.ay  = particle.ay + var_ay;
              particula.ay = particula.ay - var_ay;

              double const var_az =
                  ((((particle.pz - particula.pz) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
                     ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
                    (particle.densidad + particula.densidad - 2 * densidad)) +
                   ((particula.vz - particle.vz) * (45 / pi_sua_6) * (vis * masa))) /
                  (particle.densidad * particula.densidad);
              particle.az  = particle.az + var_az;
              particula.az = particula.az - var_az;
            }
        }
      }
    }
  }
}

void grid:: ColisionesEjeX_1(std::vector<Particle> & particles) {
  for (std::string & bloque : colisionesCx) {
    std::vector<int> & particulas_0x = blocks[bloque].particles;
    for (int id : particulas_0x) {
      Particle & particula = particles[id];
      particula.ColisionesEjeX_1();
    }
  }
}
void grid::ColisionesEjeY_1(std::vector<Particle> & particles) {
  for (std::string& bloque: colisionesCy){
    std::vector<int>& particulas_0y = blocks[bloque].particles;
    for (int id : particulas_0y) {
      Particle & particula = particles[id];
      particula.ColisionesEjeY_1();
    }
  }
}

void grid::ColisionesEjeZ_1(std::vector<Particle> & particles) {
  for (std::string& bloque: colisionesCz){
    std::vector<int>& particulas_0z = blocks[bloque].particles;
    for (int id : particulas_0z) {
      Particle & particula = particles[id];
      particula.ColisionesEjeZ_1();
    }
  }
}