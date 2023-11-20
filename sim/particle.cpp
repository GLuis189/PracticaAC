//
// Created by belen on 6/11/23.
//ey

#include "particle.hpp"

#include <math.h>
#include "variablesglobales.hpp"
#include <algorithm>
#include <cmath>


void Particle::calcularBloqueInicial(double s_x, double s_y, double s_z, int n_x, int n_y, int n_z){
    i = static_cast<int>((p_x-bmin_x)/ s_x);
    if (i> n_x -1){
      i = n_x -1;
    }
    j = static_cast<int>((p_y-bmin_y)/ s_y);
    if (j>n_y -1){
      j = n_y-1;
    }
    k = static_cast<int>((p_z-bmin_z)/ s_z);
    if (k>n_z -1){
      k = n_z -1;
    }
}

void Particle::calcularBloque(double s_x, double s_y, double s_z, int n_x, int n_y, int n_z) {
    i = std::clamp(static_cast<int>((p_x - bmin_x) / s_x), 0, n_x - 1);
    j = std::clamp(static_cast<int>((p_y - bmin_y) / s_y), 0, n_y - 1);
    k = std::clamp(static_cast<int>((p_z - bmin_z) / s_z), 0, n_z - 1);
}

void Particle::Inicializar(){
    densidad   = 0;
    a_x         = 0;
    a_y         = g_y;
    a_z         = 0;
}

void Particle::VariacionDensidad(Particle & particula, double suavizado_2){
    double const distancia    = CalcularDistancia(particula);
    if ((distancia) < (suavizado_2)) {
      double const variacion_densidad = (suavizado_2 - distancia) * (suavizado_2 - distancia) *
                           (suavizado_2 - distancia);
      densidad  = densidad + variacion_densidad;
      particula.densidad = particula.densidad + variacion_densidad;
    }
}

double Particle::CalcularDistancia(Particle & particula) const {
    double const p_dif_x   = p_x - particula.p_x;
    double const p_dif_y   = p_y - particula.p_y;
    double const p_dif_z   = p_z - particula.p_z;
    double const modulo    = std::sqrt(p_dif_x * p_dif_x + p_dif_y * p_dif_y + p_dif_z * p_dif_z);
    double const distancia = modulo * modulo;
    return distancia;
}

void Particle::VariacionAcelaracionX(Particle& particula, double suavizado, double pi_sua_6,
                                     double dist_ij, double masa){
    double const var_ax =
        ((((p_x - particula.p_x) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
           ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
          (densidad + particula.densidad - 2 * 1e3)) +
         ((particula.v_x - v_x) * (45 / pi_sua_6) * (vis * masa))) /
        (densidad * particula.densidad);
    a_x  = a_x + var_ax;
    particula.a_x = particula.a_x - var_ax;
}

void Particle::VariacionAcelaracionY(Particle & particula, double suavizado, double pi_sua_6,
                                     double dist_ij, double masa ){
    double const var_ay =
        ((((p_y - particula.p_y) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
           ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
          (densidad + particula.densidad - 2 * 1e3)) +
         ((particula.v_y - v_y) * (45 / pi_sua_6) * (vis * masa))) /
        (densidad * particula.densidad);
    a_y  = a_y + var_ay;
    particula.a_y = particula.a_y - var_ay;
}

void Particle::VariacionAcelaracionZ(Particle & particula, double suavizado, double pi_sua_6,
                                     double dist_ij, double masa ){
    double const var_az =
        ((((p_z - particula.p_z) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
           ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
          (densidad + particula.densidad - 2 * 1e3)) +
         ((particula.v_z - v_z) * (45 / pi_sua_6) * (vis * masa))) /
        (densidad * particula.densidad);
    a_z  = a_z + var_az;
    particula.a_z = particula.a_z - var_az;
}

void Particle::ColisionesEjeX0_1(){
    double const colx         = p_x + hvx * ptiempo;
    const double var_px = tparticula - (colx - bmin_x);
    if (var_px > varconst) {
      //std::cout << "Px: "<<px << " hvx: "<< hvx<<"v_x:"<<v_x <<"\n";
      a_x = a_x + (colisiones * var_px - amortiguamiento * v_x);
    }
}

void Particle::ColisionesEjeXnx_1(){
    double const colx = p_x + hvx * ptiempo;
    const double var_px = tparticula - (bmax_x - colx);
    if (var_px > varconst) {

      a_x = a_x - (colisiones * var_px + amortiguamiento * v_x);
    }
}

void Particle::ColisionesEjeY0_1(){
    double const coly         = p_y + hvy * ptiempo;
    const double var_py = tparticula - (coly - bmin_y);
    if (var_py > varconst) {
      //std::cout << "Py: "<<p_y << " hvy: "<< hvy<<"v_y:"<<v_y<<"\n";
      a_y = a_y + (colisiones * var_py - amortiguamiento * v_y);
    }
}

void Particle::ColisionesEjeYny_1(){
    double const coly = p_y + hvy * ptiempo;
    const double var_py = tparticula - (bmax_y - coly);
    if (var_py > varconst) {
      a_y = a_y - (colisiones * var_py + amortiguamiento * v_y);
    }
}

void Particle::ColisionesEjeZ0_1(){
    double const colz = p_z + hvz * ptiempo;
    const double var_pz = tparticula - (colz - bmin_z);
    if (var_pz > varconst) {
      //std::cout << "Pz: "<<p_z << " hvz: "<< hvz<<"v_z:"<<v_z<<"\n";
      a_z = a_z + (colisiones * var_pz - amortiguamiento * v_z);
    }
}
void Particle::ColisionesEjeZnz_1(){
    double const colz = p_z + hvz * ptiempo;
    const double var_pz = tparticula - (bmax_z - colz);
    if (var_pz > varconst) {
        a_z = a_z - (colisiones * var_pz + amortiguamiento * v_z);
    }
}
void Particle::MoverParticulas(int n_x, int n_y, int n_z){
    if (i == 0){
      ColisionesEjeX0_1();
    }
    else if (i == n_x -1) {
      ColisionesEjeXnx_1();
    }
    if (j == 0){
      ColisionesEjeY0_1();
    }
    else if (j == n_y -1){
      ColisionesEjeYny_1();
    }
    if (k == 0){
      ColisionesEjeZ0_1();
    }
    else if (k == n_z -1) {
      ColisionesEjeZnz_1();
    }
    p_x = p_x + hvx * ptiempo + a_x * (ptiempo * ptiempo);
    p_y = p_y + hvy * ptiempo + a_y * (ptiempo * ptiempo);
    p_z = p_z + hvz * ptiempo + a_z * (ptiempo * ptiempo);

    v_x = hvx + (a_x * ptiempo) / 2;
    v_y = hvy + (a_y * ptiempo) / 2;
    v_z = hvz + (a_z * ptiempo) / 2;

    hvx = hvx + a_x* ptiempo;
    hvy = hvy + a_y * ptiempo;
    hvz = hvz + a_z * ptiempo;

    if(i == 0 || i == n_x-1){
      ColisionesEjeX_2();
    }
    if(j == 0 || j == n_y-1){
      ColisionesEjeY_2();
    }
    if(k == 0 || k == n_z-1){
      ColisionesEjeZ_2();
    }
}

void Particle::ColisionesEjeX_2() {
    double d_x = NAN;
    if (i == 0){
      d_x = p_x - bmin_x;
    }
    else{
      d_x = bmax_x - p_x;
    }
    if(d_x<0){
      if (i == 0){
        p_x = bmin_x - d_x;
      }
      else{
        p_x = bmax_x + d_x;
      }
      v_x = -v_x;
      hvx = -hvx;
    }
}
void Particle::ColisionesEjeY_2() {
    double d_y = NAN;
    if (j == 0){
      d_y = p_y - bmin_y;
    }
    else{
      d_y = bmax_y - p_y;
    }
    if(d_y<0){
      if (j == 0){
        p_y = bmin_y - d_y;
      }
      else{
        p_y = bmax_y + d_y;
      }
      v_y = -v_y;
      hvy = -hvy;
    }
}
void Particle::ColisionesEjeZ_2() {
    double d_z = NAN;
    if (k == 0){
      d_z = p_z - bmin_z;
    }
    else{
      d_z = bmax_z - p_z;
    }
    if(d_z<0){
      if (k == 0){
        p_z = bmin_z - d_z;
      }
      else{
        p_z = bmax_z + d_z;
      }
      v_z = -v_z;
      hvz= -hvz;
    }
}
bool Particle::operator==(const Particle& other) const {
    return ide == other.ide;
}
bool Particle::operator<(const Particle& other) const {
    return ide < other.ide;
}
bool Particle::operator>(const Particle& other) const {
    return ide > other.ide;
}



