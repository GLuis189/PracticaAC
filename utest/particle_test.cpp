//
// Created by Celia on 18/11/23.
//
#include "gtest/gtest.h"
#include "../sim/particle.hpp"
#include "../sim/variablesglobales.hpp"

// Test para el método calcularBloqueInicial
TEST(ParticleTest, calcularBloqueInicial) {
  Particle p;
  p.px = 0.5; p.py = 0.5; p.pz = 0.5;
  p.calcularBloqueInicial(1.0, 1.0, 1.0, 10, 10, 10);
  // Verificar que se calcula correctamente el bloque inicial
  ASSERT_EQ(p.i, 1);
  ASSERT_EQ(p.j, 1);
  ASSERT_EQ(p.k, 1);
}

// Test para el método calcularBloque
TEST(ParticleTest, calcularBloque) {
  Particle p;
  p.px = 10.5; p.py = 10.5; p.pz = 10.5;
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
  ASSERT_DOUBLE_EQ(p.ax, 0.0);
  ASSERT_DOUBLE_EQ(p.ay, -9.8);
  ASSERT_DOUBLE_EQ(p.az, 0.0);
}

// Test para el método VariacionDensidad
TEST(ParticleTest, VariacionDensidad) {
  Particle p1, p2;
  p1.px = 0.0; p1.py = 0.0; p1.pz = 0.0;
  p2.px = 0.5; p2.py = 0.5; p2.pz = 0.5;
  p1.VariacionDensidad(p2, 1.0);
  // Verificar que se calcula correctamente la variación de la densidad
  ASSERT_DOUBLE_EQ(p1.densidad, 0);
  ASSERT_DOUBLE_EQ(p2.densidad, 0);
}

// Test para el método CalcularDistancia
TEST(ParticleTest, CalcularDistancia) {
  Particle p1, p2;
  p1.px = 0.0; p1.py = 0.0; p1.pz = 0.0;
  p2.px = 1.0; p2.py = 1.0; p2.pz = 1.0;
  double distancia = p1.CalcularDistancia(p2);
  // Verificar que se calcula correctamente la distancia
  ASSERT_DOUBLE_EQ(distancia, 3.0);
}

// Test para los métodos de colisiones
TEST(ParticleTest, Colisiones) {
  Particle p;
  p.px = -0.0661478; p.py = -0.0805976; p.pz = -0.0648605 ;
  p.hvx = -0.166259; p.hvy = 0.0432823; p.hvz =0.0442792;
  p.vx = -0.191624; p.vy = 0.0426284; p.vz = 0.0459564;
  p.ax = 0; p.ay = -9.8; p.az = 0;
  double const varx = p.px + p.hvx * ptiempo;
  double const var_px = tparticula - (varx - bmin_x);
  double const a_x            = p.ax + (colisiones * var_px - amortiguamiento * p.vx);

  double const vary = p.py + p.hvy * ptiempo;
  double const var_py = tparticula - (vary - bmin_y);
  double const a_y = p.ay + (colisiones * var_py - amortiguamiento * p.vy);

  double const varz = p.pz + p.hvz * ptiempo;
  double const var_pz = tparticula - (varz - bmin_z);
  double const a_z = p.az + (colisiones * var_pz - amortiguamiento * p.vz);
  p.ColisionesEjeX0_1();
  p.ColisionesEjeY0_1();
  p.ColisionesEjeZ0_1();
  // Verificar que se calculan correctamente las colisiones
  ASSERT_DOUBLE_EQ(p.ax, a_x);
  ASSERT_DOUBLE_EQ(p.ay, a_y);
  ASSERT_DOUBLE_EQ(p.az, a_z);
}

