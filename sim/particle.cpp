//
// Created by belen on 6/11/23.
//ey

#include "particle.hpp"


#include "variablesglobales.hpp"
#include <cmath>
#include <vector>

void Particle::Inicializar(){
    densidad   = 0;
    aceleracion.c_x        = 0;
    aceleracion.c_y        = g_y;
    aceleracion.c_z        = 0;
}

void Particle::VariacionDensidad(Particle & particula, double suavizado_2){
    double const distancia    = CalcularDistancia(particula);
    if ((distancia) < (suavizado_2)) {
      double const variacion_densidad = (suavizado_2 - distancia) * (suavizado_2 - distancia) *
                           (suavizado_2 - distancia);

      particulas_adyacentes.emplace_back(particula.ide);
      densidad  = densidad + variacion_densidad;
      particula.densidad = particula.densidad + variacion_densidad;
    }
}

double Particle::CalcularDistancia(Particle & particula) const {
    double const p_dif_x   = posicion.c_x - particula.posicion.c_x;
    double const p_dif_y   = posicion.c_y - particula.posicion.c_y;
    double const p_dif_z   = posicion.c_z - particula.posicion.c_z;
    double const modulo    = std::sqrt(p_dif_x * p_dif_x + p_dif_y * p_dif_y + p_dif_z * p_dif_z);
    double const distancia = modulo * modulo;
    return distancia;
}

void Particle::VariacionAcelaracion(Particle& particula, double suavizado, double pi_sua_6, double masa){
    double const distancia = CalcularDistancia(particula);
    double const dist_ij = std::sqrt(std::max(distancia, 1e-12));
    for(int indice = 0; indice < 3; indice++){
      double const var_a =
          ((((posicion[indice] - particula.posicion[indice]) * (15 / pi_sua_6) * ((3 * masa * presion) / 2) *
             ((suavizado - dist_ij) * (suavizado - dist_ij)) / dist_ij) *
            (densidad + particula.densidad - 2 * 1e3)) +
           ((particula.velocidad[indice] - velocidad[indice]) * (45 / pi_sua_6) * (vis * masa))) /
          (densidad * particula.densidad);
      aceleracion[indice] += var_a;
      particula.aceleracion[indice] -= var_a;
    }
}
void Particle::ColisionesEje(double bmin, double bmax, int eje){  //NOLINT
    double const col = posicion[eje] + hvelocidad[eje] * ptiempo;
    int col_p = 0;
    if(eje == 0) {
      col_p = p_i;
    } else if(eje == 1) {
      col_p = p_j;
    } else {
      col_p = p_k;
    }
    if(col_p==0){
      const double var_p = tparticula - (col - bmin);
      if (var_p > varconst) {
        aceleracion[eje] += (colisiones * var_p - amortiguamiento * velocidad[eje]);
      }
    }
    else{
      const double var_p = tparticula - (bmax - col);
      if (var_p > varconst) {
        aceleracion[eje] -= (colisiones * var_p + amortiguamiento * velocidad[eje]);
      }
    }
}

void Particle::MoverParticulas(int n_x, int n_y, int n_z){
    if (p_i == 0|| p_i == n_x -1){
        ColisionesEje(0, bmax_x, bmin_x);
    }
    if (p_j == 0||p_j == n_y -1){
        ColisionesEje(bmin_y, bmax_y, 1);
    }
    if (p_k == 0||p_k == n_z -1){
        ColisionesEje(bmin_z, bmax_x, 2);
    }
    for(int indice = 0; indice < 3; indice++){
        posicion[indice] +=  hvelocidad[indice] * ptiempo + aceleracion[indice] * (ptiempo * ptiempo);
        velocidad[indice] = hvelocidad[indice] + (aceleracion[indice] * ptiempo) / 2;
        hvelocidad[indice] += aceleracion[indice] * ptiempo;
    }

    if(p_i == 0 || p_i == n_x-1){
        ColisionesEje_2(0, bmax_x, bmin_x);
    }
    if(p_j == 0 || p_j == n_y-1){
        ColisionesEje_2(1, bmax_y, bmin_y);
    }
    if(p_k == 0 || p_k == n_z-1){
        ColisionesEje_2(2, bmax_z, bmin_z);
    }
}

void Particle::ColisionesEje_2(int eje, double bmax, double bmin){
    double col_d = 0.0;
    int col_p = 0;
    if(eje == 0) { col_p = p_i;
    } else if(eje == 1) {col_p = p_j;}
    else {col_p = p_k;}
    if(col_p == 0){
        col_d = posicion[eje] - bmin;
    }
    else{
        col_d = bmax - posicion[eje];
    }
    if(col_d<0){
        if(col_p == 0){
        posicion[eje] = bmin - col_d;
        }
        else{
        posicion[eje] = bmax + col_d;
        }
        velocidad[eje] = -velocidad[eje];
        hvelocidad[eje] = -hvelocidad[eje];
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



