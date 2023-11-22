//
// Created by luis on 30/10/23.
//
// test_grid.cpp

#include "gtest/gtest.h"
#include <cmath>
#include "../sim/grid.hpp"
#include "../sim/variablesglobales.hpp"

TEST(GridTest, GenerarBloques) {
  double const suavizado = 1.0;
  grid const grid(suavizado);
  // Verificar que se generan la cantidad correcta de bloques
  int const expected_num_blocks = grid.n_x * grid.n_y * grid.n_z;
  EXPECT_EQ(grid.blocks.size(), expected_num_blocks);
}

TEST(GridTest, GenerarClaveBloque) {
  grid const grid(1.0);
  EXPECT_EQ(grid.generarClaveBloque(1, 2, 3), 10203);
}

TEST(GridTest, EsValido) {
  grid const grid(1.0);
  EXPECT_TRUE(grid.esValido(0, 10));
  EXPECT_FALSE(grid.esValido(-1, 10));
  EXPECT_FALSE(grid.esValido(10, 10));
}

// Test para el método calcularBloquesAdyacentes
TEST(GridTest, calcularBloquesAdyacentes) {
  double const suavizado = suavizadot;
  grid grid(suavizado);
  // Verificar que este método calcula correctamente los bloques adyacentes para un bloque dado
  int const block_key2 = grid::generarClaveBloque(5, 5, 5);
  ASSERT_EQ(grid.blocks[block_key2].bloques_ady.size(), 27);
}

TEST(GridTest, CalcularBloque) {
  double const suavizado = suavizadot;
  grid const grid(suavizado);
  Particle part;
  part.posicion.c_x = n_05;
  part.posicion.c_y = n_05;
  part.posicion.c_z = n_05;
  grid.calcularBloque(part);

  EXPECT_EQ(part.p_i, 21);
  EXPECT_EQ(part.p_j, 30);
  EXPECT_EQ(part.p_k, 21);
}

TEST(GridTest, CalcularBloqueInicial) {
  double const suavizado = suavizadot;
  grid const grid(suavizado);
  Particle part;
  part.posicion.c_x = 0.0;
  part.posicion.c_y = 0.0;
  part.posicion.c_z = 0.0;
  grid.calcularBloqueInicial(part);

  EXPECT_EQ(part.p_i, 11);
  EXPECT_EQ(part.p_j, 13);
  EXPECT_EQ(part.p_k, 11);
}
TEST(GridTest, CambiarBloque) {
  double const suavizado = suavizadot;
  double const ceroseis = 0.6;
  grid myGrid(suavizado);
  Particle particle;
  particle.posicion.c_x = 0.0;
  particle.posicion.c_y = 0.0;
  particle.posicion.c_z = 0.0;
  myGrid.calcularBloqueInicial(particle);
  const int i_anterior = particle.p_i;
  const int j_anterior = particle.p_j;
  const int k_anterior = particle.p_k;
  particle.posicion.c_x = ceroseis;
  particle.posicion.c_y = ceroseis;
  particle.posicion.c_z = ceroseis;
  myGrid.calcularBloque(particle);

  myGrid.CambiarBloque(particle, i_anterior, j_anterior, k_anterior);
  const int old_block_key = grid::generarClaveBloque(i_anterior, j_anterior, k_anterior);
  const int new_block_key = grid::generarClaveBloque(particle.p_i, particle.p_j, particle.p_k);

  EXPECT_FALSE(old_block_key == new_block_key);
}
// Prueba para la función calcularDensidades
TEST(GridTest, CalcularDensidades) {
  const double masa = 1.0;
  const double suavizado = 1.0;
  const double suavizado_2 = suavizado * suavizado;
  grid grid(suavizadot);
  std::vector<Particle> particles;
  // Llenar el vector de partículas con algunos valores de prueba
  for(int indice = 0; indice < 3; indice++){ const Particle particle; particles.emplace_back(particle);}

  std::vector<Particle> particles_copy = particles;

  grid.calcularDensidades(particles, masa, suavizado, suavizado_2);

  for (Particle& particle : particles_copy) {
    for (Particle& particula : particles_copy) {
      if (particula > particle) { particle.VariacionDensidad(particula, suavizado_2); }
    }
     particle.densidad = (particle.densidad + pow(suavizado, n_6)) * n_315 * masa/ (n_64 * M_PI * pow(suavizado, n_9));
  }

  // Comprobar que las densidades de las partículas se han calculado correctamente
  ASSERT_DOUBLE_EQ(particles[0].densidad, particles_copy[0].densidad);
  ASSERT_DOUBLE_EQ(particles[1].densidad, particles_copy[1].densidad);
  ASSERT_DOUBLE_EQ(particles[2].densidad, particles_copy[2].densidad);
}
