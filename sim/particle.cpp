//
// Created by belen on 6/11/23.
//ey

#include "particle.hpp"
#include <iostream>
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

bool Particle::operator==(const Particle& other) const {
    return id == other.id;
}