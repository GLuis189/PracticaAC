//
// Created by belen on 6/11/23.
//ey

#include "particle.hpp"
#include <iostream>
#include <cmath>
#include <vector>

void Particle::calcularBloque(Particle& particle, float bmin_x, float sx, float bmin_y, float sy, float bmin_z, float sz, int nx, int ny, int nz){
    particle.i = static_cast<int>((particle.px-bmin_x)/sx);
    if (particle.i>nx -1){
      particle.i = nx -1;
    }
    particle.j = static_cast<int>((particle.py-bmin_y)/sy);
    if (particle.j>ny -1){
      particle.j = ny-1;
    }
    particle.k = static_cast<int>((particle.pz-bmin_z)/sz);
    if (particle.k>nz -1){
      particle.k = nz -1;
    }
}

/*void Particle::calcularDensidad(Particle& p_i, std::vector<Particle>& particles, int numparticulas, float suavizado) {
    for (int pos = p_i.id +1; pos<numparticulas; pos++) {
      if (p_i.i == particles[pos].i - 1 || p_i.i == particles[pos].i || p_i.i == particles[pos].i+1) {
        if (p_i.j == particles[pos].j - 1 || p_i.j == particles[pos].j || p_i.j == particles[pos].j+1) {
          if (p_i.k == particles[pos].j - 1 || p_i.k == particles[pos].k || p_i.k == particles[pos].k+1) {
            double p_dif = p_i.densidad - particles[pos].densidad;
            double variacion_densidad = 0;
            if (pow(p_dif,2) < pow(suavizado,2)){
              variacion_densidad = (pow(suavizado,2)-pow(p_dif,2));
            }
            p_i.densidad = p_i.densidad + pow(variacion_densidad,3);
            particles[pos].densidad = particles[pos].densidad + pow(variacion_densidad,3);
          }
        }
      }
    }
}*/

bool Particle::operator==(const Particle& other) const {
    return id == other.id;
}
bool Particle::operator<(const Particle& other) const {
    return id < other.id;
}
bool Particle::operator>(const Particle& other) const {
    return id > other.id;
}

