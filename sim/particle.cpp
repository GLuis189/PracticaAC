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

double Particle::VariacionAcelaracionX(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa){
    double const var_ax =
        ((((px - particula.px) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
           ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
          (densidad + particula.densidad - 2 * densidad)) +
         ((particula.vx - vx) * (45 / pi_sua_6) * (vis * masa))) /
        (densidad * particula.densidad);
    return var_ax;
}

void Particle::VariacionAcelaracionY(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa ){
    double const var_ay =
        ((((py - particula.py) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
           ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
          (densidad + particula.densidad - 2 * densidad)) +
         ((particula.vy - vy) * (45 / pi_sua_6) * (vis * masa))) /
        (densidad * particula.densidad);
    ay  = ay + var_ay;
    particula.ay = particula.ay - var_ay;
}

void Particle::VariacionAcelaracionZ(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa ){
    double const var_az =
        ((((pz - particula.pz) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
           ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
          (densidad + particula.densidad - 2 * densidad)) +
         ((particula.vz - vz) * (45 / pi_sua_6) * (vis * masa))) /
        (densidad * particula.densidad);
    az  = az + var_az;
    particula.az = particula.az - var_az;
}

void Particle::ColisionesEjeX_1(){
    double x = px + hvx * ptiempo;
    double var_px;
    if (i == 0) {
      var_px = tparticula - (x - bmin_x);
    } else {
      var_px = tparticula - (bmax_x - x);
    }
    if (var_px > 1e-10) {
      if (i == 0) {
        ax = ax + (colisiones * var_px - amortiguamiento * vx);
      } else {
        ax = ax - (colisiones * var_px + amortiguamiento * vx);
      }
    }
}
void Particle::ColisionesEjeY_1(){
    double y = py + hvy * ptiempo;
    double var_py;
    if (j == 0) {
      var_py = tparticula - (y - bmin_y);
    } else {
      var_py = tparticula - (bmax_y - y);
    }
    if (var_py > 1e-10) {
      if (j == 0) {
        ay = ay + (colisiones * var_py - amortiguamiento * vy);
      } else {
        ay = ay - (colisiones * var_py + amortiguamiento * vy);
      }
    }
}
void Particle::ColisionesEjeZ_1(){
    double z = pz + hvz * ptiempo;
    double var_pz;
    if (k == 0) {
      var_pz = tparticula - (z - bmin_z);
    } else {
      var_pz = tparticula - (bmax_z - z);
    }
    if (var_pz > 1e-10) {
      if (k == 0) {
        az = az + (colisiones * var_pz - amortiguamiento * vz);
      } else {
        az = az - (colisiones * var_pz + amortiguamiento * vz);
      }
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

