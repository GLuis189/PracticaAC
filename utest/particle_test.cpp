//
// Created by Celia on 18/11/23.
//
#include "gtest/gtest.h"
#include "../sim/particle.hpp"
#include "../sim/variablesglobales.hpp"

// Test para el método calcularBloqueInicial
TEST(ParticleTest, calcularBloqueInicial) {
  Particle p;
  p.p_x = 0.5; p.p_y = 0.5; p.p_z = 0.5;
  p.calcularBloqueInicial(1.0, 1.0, 1.0, 10, 10, 10);
  // Verificar que se calcula correctamente el bloque inicial
  ASSERT_EQ(p.i, 1);
  ASSERT_EQ(p.j, 1);
  ASSERT_EQ(p.k, 1);
}

// Test para el método calcularBloque
TEST(ParticleTest, calcularBloque) {
  Particle p;
  p.p_x = 10.5; p.p_y = 10.5; p.p_z = 10.5;
  p.calcularBloque(1.0, 1.0, 1.0, 10, 10, 10);
  // Verificar que se calcula correctamente el bloque y se ajustan los índices para que estén dentro de los límites
  ASSERT_EQ(p.i, 9);
  ASSERT_EQ(p.j, 9);
  ASSERT_EQ(p.k, 9);
}

// Test para el método Inicializar
TEST(ParticleTest, Inicializar) {
  Particle p;
  p.Inicializar();
  // Verificar que se inicializan correctamente las propiedades de la partícula
  ASSERT_DOUBLE_EQ(p.densidad, 0.0);
  ASSERT_DOUBLE_EQ(p.a_x, 0.0);
  ASSERT_DOUBLE_EQ(p.a_y, -9.8);
  ASSERT_DOUBLE_EQ(p.a_z, 0.0);
}

// Test para el método VariacionDensidad
TEST(ParticleTest, VariacionDensidad) {
  Particle p1, p2;
  p1.p_x = 0.0; p1.p_y = 0.0; p1.p_z = 0.0;
  p2.p_x = 0.5; p2.p_y = 0.5; p2.p_z = 0.5;
  p1.VariacionDensidad(p2, 1.0);
  // Verificar que se calcula correctamente la variación de la densidad
  ASSERT_DOUBLE_EQ(p1.densidad, 0);
  ASSERT_DOUBLE_EQ(p2.densidad, 0);
}

// Test para el método CalcularDistancia
TEST(ParticleTest, CalcularDistancia) {
  Particle p1, p2;
  p1.p_x = 0.0; p1.p_y = 0.0; p1.p_z = 0.0;
  p2.p_x = 1.0; p2.p_y = 1.0; p2.p_z = 1.0;
  double distancia = p1.CalcularDistancia(p2);
  // Verificar que se calcula correctamente la distancia
  ASSERT_DOUBLE_EQ(distancia, 3.0);
}

// Test para los métodos de colisiones
TEST(ParticleTest, Colisiones) {
  Particle p;
  p.p_x = -0.0661478; p.p_y = -0.0805976; p.p_z = -0.0648605 ;
  p.hvx = -0.166259; p.hvy = 0.0432823; p.hvz =0.0442792;
  p.v_x = -0.191624; p.v_y = 0.0426284; p.v_z = 0.0459564;
  p.a_x = 0; p.a_y = -9.8; p.a_z = 0;
  double const varx = p.p_x + p.hvx * ptiempo;
  double const var_px = tparticula - (varx - bmin_x);
  double const a_x            = p.a_x + (colisiones * var_px - amortiguamiento * p.v_x);

  double const vary = p.p_y + p.hvy * ptiempo;
  double const var_py = tparticula - (vary - bmin_y);
  double const a_y = p.a_y + (colisiones * var_py - amortiguamiento * p.v_y);

  double const varz = p.p_z + p.hvz * ptiempo;
  double const var_pz = tparticula - (varz - bmin_z);
  double const a_z = p.a_z + (colisiones * var_pz - amortiguamiento * p.v_z);
  p.ColisionesEjeX0_1();
  p.ColisionesEjeY0_1();
  p.ColisionesEjeZ0_1();
  // Verificar que se calculan correctamente las colisiones
  ASSERT_DOUBLE_EQ(p.a_x, a_x);
  ASSERT_DOUBLE_EQ(p.a_y, a_y);
  ASSERT_DOUBLE_EQ(p.a_z, a_z);
}

