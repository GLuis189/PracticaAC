//
// Created by belen on 6/11/23.
//ey

#include "particle.hpp"
#include "variablesglobales.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

void Particle::calcularBloqueInicial(double sx, double sy, double sz, int nx, int ny, int nz){
    i = static_cast<int>((px-bmin_x)/sx);
    if (i>nx -1){
      i = nx -1;
    }
    j = static_cast<int>((py-bmin_y)/sy);
    if (j>ny -1){
      j = ny-1;
    }
    k = static_cast<int>((pz-bmin_z)/sz);
    if (k>nz -1){
      k = nz -1;
    }
}

void Particle::calcularBloque(double sx, double sy, double sz, int nx, int ny, int nz){
    i = static_cast<int>((px-bmin_x)/sx);
    if (i>nx -1){
      i = nx -1;
    }
    if (i<0){
      i = 0;
    }
    j = static_cast<int>((py-bmin_y)/sy);
    if (j>ny -1){
      j = ny-1;
    }
    if (j<0){
      j = 0;
    }
    k = static_cast<int>((pz-bmin_z)/sz);
    if (k>nz -1){
      k = nz -1;
    }
    if (k<0){
      k = 0;
    }
}

void Particle::Inicializar(){
    densidad   = 0;
    ax         = 0;
    ay         = -9.8;
    az         = 0;
}

void Particle::VariacionDensidad(Particle & particula, double suavizado_2){
    double const distancia    = CalcularDistancia(particula);
    if ((distancia) < (suavizado_2)) {
      double variacion_densidad = (suavizado_2 - distancia) * (suavizado_2 - distancia) *
                           (suavizado_2 - distancia);
      densidad  = densidad + variacion_densidad;
      particula.densidad = particula.densidad + variacion_densidad;
    }
}

double Particle::CalcularDistancia(Particle & particula) {
    double const p_dif_x   = px - particula.px;
    double const p_dif_y   = py - particula.py;
    double const p_dif_z   = pz - particula.pz;
    double const modulo    = std::sqrt(p_dif_x * p_dif_x + p_dif_y * p_dif_y + p_dif_z * p_dif_z);
    double const distancia = modulo * modulo;
    return distancia;
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

