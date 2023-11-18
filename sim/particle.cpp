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
void Particle::MoverParticulas(){
    px = px + hvx * ptiempo + ax * (ptiempo * ptiempo);
    py = py + hvy * ptiempo + ay * (ptiempo * ptiempo);
    pz = pz + hvz * ptiempo + az * (ptiempo * ptiempo);

    vx = hvx + (ax * ptiempo) / 2;
    vy = hvy + (ay * ptiempo) / 2;
    vz = hvz + (az * ptiempo) / 2;

    hvx = hvx + ax * ptiempo;
    hvy = hvy + ay * ptiempo;
    hvz = hvz + az * ptiempo;
}

void Particle::ColisionesEjeX_2() {
    double d_x;
    if (i == 0){
      d_x = px - bmin_x;
    }
    else{
      d_x = bmax_x - px;
    }
    if(d_x<0){
      if (i == 0){
        px = bmin_x - d_x;
      }
      else{
        px = bmax_x + d_x;
      }
      vx = -vx;
      hvx = -hvx;
    }
}
void Particle::ColisionesEjeY_2() {
    double d_y;
    if (j == 0){
      d_y = py - bmin_y;
    }
    else{
      d_y = bmax_y - py;
    }
    if(d_y<0){
      if (j == 0){
        py = bmin_y - d_y;
      }
      else{
        py = bmax_y + d_y;
      }
      vy = -vy;
      hvy = -hvy;
    }
}
void Particle::ColisionesEjeZ_2() {
    double d_z;
    if (k == 0){
      d_z = pz - bmin_z;
    }
    else{
      d_z = bmax_z - pz;
    }
    if(d_z<0){
      if (k == 0){
        pz = bmin_z - d_z;
      }
      else{
        pz = bmax_z + d_z;
      }
      vz = -vz;
      hvz= -hvz;
    }
}
bool Particle::operator==(const Particle& other) const {
    return id == other.id;
}
bool Particle::operator<(const Particle& other) const {
    return id < other.id;
}
bool Particle::operator>(const Particle& other) const {
    return id > other.id;
}

