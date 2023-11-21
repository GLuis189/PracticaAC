//
// Created by Celia on 18/11/23.
//
#include "gtest/gtest.h"
#include "../sim/particle.hpp"
#include "../sim/variablesglobales.hpp"

// Test para el método calcularBloqueInicial
TEST(ParticleTest, calcularBloqueInicial) {
  Particle p;
  p.posicion.c_x = 0.5; p.posicion.c_y = 0.5; p.posicion.c_z = 0.5;
  p.calcularBloqueInicial(1.0, 1.0, 1.0, 10, 10, 10);
  // Verificar que se calcula correctamente el bloque inicial
  ASSERT_EQ(p.p_i, 1);
  ASSERT_EQ(p.p_j, 1);
  ASSERT_EQ(p.p_k, 1);
}

// Test para el método calcularBloque
TEST(ParticleTest, calcularBloque) {
  Particle p;
  p.posicion.c_x = 10.5; p.posicion.c_y = 10.5; p.posicion.c_z = 10.5;
  p.calcularBloque(1.0, 1.0, 1.0, 10, 10, 10);
  // Verificar que se calcula correctamente el bloque y se ajustan los índices para que estén dentro de los límites
  ASSERT_EQ(p.p_i, 9);
  ASSERT_EQ(p.p_j, 9);
  ASSERT_EQ(p.p_k, 9);
}

// Test para el método Inicializar
TEST(ParticleTest, Inicializar) {
  Particle p;
  p.Inicializar();
  // Verificar que se inicializan correctamente las propiedades de la partícula
  ASSERT_DOUBLE_EQ(p.densidad, 0.0);
  ASSERT_DOUBLE_EQ(p.aceleracion.c_x, 0.0);
  ASSERT_DOUBLE_EQ(p.aceleracion.c_y, -9.8);
  ASSERT_DOUBLE_EQ(p.aceleracion.c_z, 0.0);
}

// Test para el método VariacionDensidad
TEST(ParticleTest, VariacionDensidad) {
  Particle p1, p2;
  p1.posicion.c_x = 0.0; p1.posicion.c_y = 0.0; p1.posicion.c_z = 0.0;
  p2.posicion.c_x = 0.5; p2.posicion.c_y = 0.5; p2.posicion.c_z = 0.5;
  p1.VariacionDensidad(p2, 1.0);
  // Verificar que se calcula correctamente la variación de la densidad
  ASSERT_DOUBLE_EQ(p1.densidad, 0);
  ASSERT_DOUBLE_EQ(p2.densidad, 0);
}

// Test para el método CalcularDistancia
TEST(ParticleTest, CalcularDistancia) {
  Particle p1, p2;
  p1.posicion.c_x = 0.0; p1.posicion.c_y = 0.0; p1.posicion.c_z = 0.0;
  p2.posicion.c_x = 1.0; p2.posicion.c_y = 1.0; p2.posicion.c_z = 1.0;
  double distancia = p1.CalcularDistancia(p2);
  // Verificar que se calcula correctamente la distancia
  ASSERT_DOUBLE_EQ(distancia, 3.0);
}

// Test para los métodos de colisiones
TEST(ParticleTest, Colisiones) {
  Particle p;
  p.posicion.c_x = -0.0661478; p.posicion.c_y = -0.0805976; p.posicion.c_z = -0.0648605 ;
  p.hvelocidad.c_x = -0.166259; p.hvelocidad.c_y = 0.0432823; p.hvelocidad.c_z =0.0442792;
  p.velocidad.c_x = -0.191624; p.velocidad.c_y = 0.0426284; p.velocidad.c_z = 0.0459564;
  p.aceleracion.c_x = 0; p.aceleracion.c_y = -9.8; p.aceleracion.c_z = 0;

  double const varx = p.posicion.c_x + p.hvelocidad.c_x * ptiempo;
  double const var_px = tparticula - (varx - bmin_x);
  double const a_x = p.aceleracion.c_x + (colisiones * var_px - amortiguamiento * p.velocidad.c_x);

  double const vary = p.posicion.c_y + p.hvelocidad.c_y * ptiempo;
  double const var_py = tparticula - (vary - bmin_y);
  double const a_y = p.aceleracion.c_y + (colisiones * var_py - amortiguamiento * p.velocidad.c_y);

  double const varz = p.posicion.c_z + p.hvelocidad.c_z * ptiempo;
  double const var_pz = tparticula - (varz - bmin_z);
  double const a_z = p.aceleracion.c_z + (colisiones * var_pz - amortiguamiento * p.velocidad.c_z);
  p.ColisionesEje_1(0, bmax_x, bmin_x);
  p.ColisionesEje_1(1, bmax_y, bmin_y);
  p.ColisionesEje_1(2, bmax_z, bmin_z);
  // Verificar que se calculan correctamente las colisiones
  ASSERT_DOUBLE_EQ(p.aceleracion.c_x, a_x);
  ASSERT_DOUBLE_EQ(p.aceleracion.c_y, a_y);
  ASSERT_DOUBLE_EQ(p.aceleracion.c_z, a_z);
}

