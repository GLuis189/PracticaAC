//
// Created by Celia on 18/11/23.
//
#include "../sim/particle.hpp"
#include "../sim/variablesglobales.hpp"

#include "gtest/gtest.h"
#include <cmath>

const float k_1 = 0.5;
const float k_2 = 0.6;

TEST(ParticleTest, Inicializar) {
  Particle particle;
  particle.Inicializar();
  EXPECT_EQ(particle.densidad, 0);
  EXPECT_EQ(particle.aceleracion.c_x, 0);
  EXPECT_EQ(particle.aceleracion.c_y, g_y);
  EXPECT_EQ(particle.aceleracion.c_z, 0);
}

TEST(ParticleTest, CalcularDistancia) {
  Particle particle1; Particle particle2;
  particle1.posicion.c_x = k_1;
  particle1.posicion.c_y = k_1;
  particle1.posicion.c_z = k_1;
  particle2.posicion.c_x = k_2;
  particle2.posicion.c_y = k_2;
  particle2.posicion.c_z = k_2;
  double const modulo    = std::sqrt(0.1 * 0.1 + 0.1 * 0.1 + 0.1 * 0.1);
  double const distancia = modulo * modulo;
  EXPECT_NEAR(particle1.CalcularDistancia(particle2), distancia, 1e-9);
}

TEST(ParticleTest, VariacionDensidad) {
  Particle particle1; Particle particle2;
  particle1.posicion = {0.0, 0.0, 0.0};
  particle2.posicion = {k_2, k_2, k_2};
  particle1.VariacionDensidad(particle2, 1.0);
  // Verificar que se calcula correctamente la variación de la densidad
  ASSERT_DOUBLE_EQ(particle1.densidad, 0);
  ASSERT_DOUBLE_EQ(particle2.densidad, 0);
}

TEST(ParticleTest, VariacionAceleracion) {
  Particle particle1; Particle particle2;
  particle1.posicion = {0.0, 0.0, 0.0};
  particle2.posicion = {1.0, 1.0, 1.0};
  particle1.velocidad = {0.0, 0.0, 0.0};
  particle2.velocidad = {0.0, 0.0, 0.0};
  particle1.aceleracion = {0.0, g_y, 0.0};
  particle1.aceleracion = {0.0, g_y, 0.0};
  double const suavizado = 1.0;
  double const pi_sua_6 = M_PI * pow(suavizado, n_6);
  double const masa = 1.0;
  particle1.VariacionAcelaracion(particle2, suavizado, pi_sua_6, masa);
  // Comprueba los resultados
  for (int i = 0; i < 3; i++) {
    EXPECT_DOUBLE_EQ(particle1.aceleracion[i], -particle2.aceleracion[i]);
  }
}


/*
// Test para el método calcularBloqueInicial
TEST(ParticleTest, calcularBloqueInicial) {
  Particle particle;
  particle.posicion.c_x = k_1; particle.posicion.c_y = k_1; particle.posicion.c_z = k_1;
  particle.calcularBloqueInicial(1.0, 1.0, 1.0, 10, 10, 10);
  // Verificar que se calcula correctamente el bloque inicial
  ASSERT_EQ(particle.p_i, 1);
  ASSERT_EQ(particle.p_j, 1);
  ASSERT_EQ(particle.p_k, 1);
}

// Test para el método calcularBloque
TEST(ParticleTest, calcularBloque) {
  Particle particle;
  particle.posicion.c_x = 10.5; particle.posicion.c_y = 10.5; particle.posicion.c_z = 10.5;
  particle.calcularBloque(1.0, 1.0, 1.0, 10, 10, 10);
  // Verificar que se calcula correctamente el bloque y se ajustan los índices para que estén dentro de los límites
  ASSERT_EQ(particle.p_i, 9);
  ASSERT_EQ(particle.p_j, 9);
  ASSERT_EQ(particle.p_k, 9);
}

// Test para el método Inicializar
TEST(ParticleTest, Inicializar) {
  Particle particle;
  particle.Inicializar();
  // Verificar que se inicializan correctamente las propiedades de la partícula
  ASSERT_DOUBLE_EQ(particle.densidad, 0.0);
  ASSERT_DOUBLE_EQ(particle.aceleracion.c_x, 0.0);
  ASSERT_DOUBLE_EQ(particle.aceleracion.c_y, -9.8);
  ASSERT_DOUBLE_EQ(particle.aceleracion.c_z, 0.0);
}

// Test para el método VariacionDensidad
TEST(ParticleTest, VariacionDensidad) {
  Particle particle1, particle2;
  particle1.posicion.c_x = 0.0; particle1.posicion.c_y = 0.0; particle1.posicion.c_z = 0.0;
  particle2.posicion.c_x = 0.5; particle2.posicion.c_y = 0.5; particle2.posicion.c_z = 0.5;
  particle1.VariacionDensidad(particle2, 1.0);
  // Verificar que se calcula correctamente la variación de la densidad
  ASSERT_DOUBLE_EQ(particle1.densidad, 0);
  ASSERT_DOUBLE_EQ(particle2.densidad, 0);
}

// Test para el método CalcularDistancia
TEST(ParticleTest, CalcularDistancia) {
  Particle particle1, particle2;
  particle1.posicion.c_x = 0.0; particle1.posicion.c_y = 0.0; particle1.posicion.c_z = 0.0;
  particle2.posicion.c_x = 1.0; particle2.posicion.c_y = 1.0; particle2.posicion.c_z = 1.0;
  double distancia = particle1.CalcularDistancia(particle2);
  // Verificar que se calcula correctamente la distancia
  ASSERT_DOUBLE_EQ(distancia, 3.0);
}

// Test para los métodos de colisiones
TEST(ParticleTest, Colisiones) {
  Particle particle;
  particle.posicion.c_x = -0.0661478; particle.posicion.c_y = -0.0805976; particle.posicion.c_z = -0.0648605 ;
  particle.hvelocidad.c_x = -0.166259; particle.hvelocidad.c_y = 0.0432823; particle.hvelocidad.c_z =0.0442792;
  particle.velocidad.c_x = -0.191624; particle.velocidad.c_y = 0.0426284; particle.velocidad.c_z = 0.0459564;
  particle.aceleracion.c_x = 0; particle.aceleracion.c_y = -9.8; particle.aceleracion.c_z = 0;

  double const varx = particle.posicion.c_x + particle.hvelocidad.c_x * ptiempo;
  double const var_px = tparticula - (varx - bmin_x);
  double const a_x = particle.aceleracion.c_x + (colisiones * var_px - amortiguamiento * particle.velocidad.c_x);

  double const vary = particle.posicion.c_y + particle.hvelocidad.c_y * ptiempo;
  double const var_py = tparticula - (vary - bmin_y);
  double const a_y = particle.aceleracion.c_y + (colisiones * var_py - amortiguamiento * particle.velocidad.c_y);

  double const varz = particle.posicion.c_z + particle.hvelocidad.c_z * ptiempo;
  double const var_pz = tparticula - (varz - bmin_z);
  double const a_z = particle.aceleracion.c_z + (colisiones * var_pz - amortiguamiento * particle.velocidad.c_z);

  particle.ColisionesEje_1(0, bmax_x, bmin_x);
  particle.ColisionesEje_1(1, bmax_y, bmin_y);
  particle.ColisionesEje_1(2, bmax_z, bmin_z);

  // Verificar que se calculan correctamente las colisiones
  ASSERT_DOUBLE_EQ(particle.aceleracion.c_x, a_x);
  ASSERT_DOUBLE_EQ(particle.aceleracion.c_y, a_y);
  ASSERT_DOUBLE_EQ(particle.aceleracion.c_z, a_z);
}

*/

