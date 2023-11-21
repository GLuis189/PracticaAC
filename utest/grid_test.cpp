//
// Created by luis on 30/10/23.
//
// test_grid.cpp

#include "gtest/gtest.h"
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
/*
TEST(GridTest, CambiarBloque) {
  double const suavizado = suavizadot;
  grid myGrid(suavizado);
  Particle particle;
  particle.posicion.c_x = 0.0;
  particle.posicion.c_y = 0.0;
  particle.posicion.c_z = 0.0;
  myGrid.calcularBloqueInicial(particle);
  const int i_anterior = particle.p_i;
  const int j_anterior = particle.p_j;
  const int k_anterior = particle.p_k;
  particle.posicion.c_x = 0.6;
  particle.posicion.c_y = 0.6;
  particle.posicion.c_z = 0.6;
  myGrid.calcularBloque(particle);

  myGrid.CambiarBloque(particle, i_anterior, j_anterior, k_anterior);
  int old_block_key = myGrid.generarClaveBloque(i_anterior, j_anterior, k_anterior);
  int new_block_key = myGrid.generarClaveBloque(particle.p_i, particle.p_j, particle.p_k);

  EXPECT_FALSE(old_block_key == new_block_key);
}

TEST(GridTest, CalcularDensidades) {
  double suavizado = suavizadot;
  grid grid(suavizado);
  std::vector<Particle>& particles;
  double masa = 1;
  double suavizado_2 = suavizado * suavizado;
  // Llenar el vector de partículas con algunos valores de prueba
  // particles.push_back(Particle(...));
  // particles.push_back(Particle(...));
  // ...

  grid.calcularDensidades(particles, masa, suavizado, suavizado_2);

  // Comprobar que las densidades de las partículas se han calculado correctamente
  // ASSERT_DOUBLE_EQ(particles[0].densidad, valor_esperado);
  // ASSERT_DOUBLE_EQ(particles[1].densidad, valor_esperado);
  // ...
}
*/
// Test para el método CambiarBloque
/*TEST(GridTest, CambiarBloque) {
  double const suavizado = suavizadot;
  grid const myGrid(suavizado);
  Particle const particle;
  int const block_key = generarClaveBloque(5, 5, 5);
  myGrid.blocks[block_key].removeParticle(particle.ide);
  int const block_key2 = generarClaveBloque(particle.p_i, particle.p_j, particle.p_k);
  blocks[block_key2].addParticle(particle.ide);
  ASSERT_EQ(myGrid.blocks[block_key2].particles.size(), 1);
  ASSERT_EQ(myGrid.blocks[block_key].particles.size(), 0);
}*/

 /*
// Test para el método ColisionesEjeX_1
TEST(GridTest, ColisionesEjeX_1) {
  int nx = 15;
  int ny = 21;
  int nz = 15;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.i = 0; p.j = 0; p.k = 0;
  p.p_x =  -0.0661478;
  p.hvx =  -0.166259;
  p.v_x = -0.191624;
  p.a_x = 0;
  particles.push_back(p);
  myGrid.blocks[myGrid.generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeX_1
  myGrid.ColisionesEjeX_1(particles);
  double x = p.p_x + p.hvx * ptiempo;
  double var_px = tparticula - (x - bmin_x);
  double aceleracion = p.a_x + (colisiones * var_px - amortiguamiento *p.v_x);
  // Verificar que la partícula ha colisionado correctamente
  ASSERT_DOUBLE_EQ(particles[0].a_x, aceleracion);
}
TEST(GridTest, No_ColisionesEjeX_1) {
  int nx = 15;
  int ny = 21;
  int nz = 15;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.i = 10; p.j = 0; p.k = 0;
  p.p_x =  -0.0661478;
  p.hvx =  -0.0661478;
  p.a_x = 0;
  particles.push_back(p);
  myGrid.blocks[myGrid.generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeX_1
  myGrid.ColisionesEjeX_1(particles);
  // Verificar que la partícula no ha colisionado
  ASSERT_DOUBLE_EQ(particles[0].a_x, 0);
}
// Test para el método ColisionesEjeY_1
TEST(GridTest, ColisionesEjeY_1) {
  int nx = 15;
  int ny = 21;
  int nz = 15;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.i = 0; p.j = 0; p.k = 0;
  p.p_y =  -0.0805976;
  p.hvy =  0.0432823;
  p.v_y = 0.0426284;
  p.a_y = -9.8;
  particles.push_back(p);
  myGrid.blocks[myGrid.generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeY_1
  myGrid.ColisionesEjeY_1(particles);
  double y = p.p_y + p.hvy * ptiempo;
  double var_py = tparticula - (y - bmin_y);
  double aceleracion = p.a_y + (colisiones * var_py - amortiguamiento *p.v_y);
  // Verificar que la partícula ha colisionado correctamente
  ASSERT_DOUBLE_EQ(particles[0].a_y, aceleracion);
}
TEST(GridTest, No_ColisionesEjeY_1) {
  int nx = 15;
  int ny = 21;
  int nz = 15;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.i   = 0;
  p.j   = 10;
  p.k   = 0;
  p.p_y  = -0.0661478;
  p.hvy = -0.0661478;
  p.a_y  = 0;
  particles.push_back(p);
  myGrid.blocks[grid::generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeY_1
  myGrid.ColisionesEjeY_1(particles);
  // Verificar que la partícula no ha colisionado
  ASSERT_DOUBLE_EQ(particles[0].a_y, 0);
}

// Test para el método ColisionesEjeZ_1
TEST(GridTest, ColisionesEjeZ_1) {
  int nx = 15;
  int ny = 21;
  int nz = 15;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.ide = 0;
  p.i   = 0;
  p.j   = 0;
  p.k   = 0;
  p.p_z  = -0.0648605;
  p.hvz = 0.0442792;
  p.v_z  = 0.0459564;
  p.a_z  = 0;
  particles.push_back(p);
  myGrid.blocks[myGrid.generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeZ_1
  myGrid.ColisionesEjeZ_1(particles);
  double z           = p.p_z + p.hvz * ptiempo;
  double var_pz      = tparticula - (z - bmin_z);
  double aceleracion = p.a_z + (colisiones * var_pz - amortiguamiento * p.v_z);
  // Verificar que la partícula ha colisionado correctamente
  ASSERT_DOUBLE_EQ(particles[0].a_z, aceleracion);
}

TEST(GridTest, No_ColisionesEjeZ_1) {
  int nx = 15;
  int ny = 21;
  int nz = 15;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.ide = 0;
  p.i   = 0;
  p.j   = 0;
  p.k   = 10;
  p.a_z  = 0;
  particles.push_back(p);
  myGrid.blocks[myGrid.generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeZ_1
  myGrid.ColisionesEjeZ_1(particles);
  // Verificar que la partícula no ha colisionado
  ASSERT_DOUBLE_EQ(particles[0].a_z, 0);
}

// Test para el método ColisionesEjeX_2
TEST(GridTest, ColisionesEjeX_2) {
  int nx = 10;
  int ny = 10;
  int nz = 10;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.i = 0; p.j = 0; p.k = 0;
  particles.push_back(p);
  myGrid.blocks[myGrid.generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeX_2
  myGrid.ColisionesEjeX_2(particles);
  // Verificar que la partícula ha colisionado correctamente
  ASSERT_DOUBLE_EQ(particles[0].a_x, 1 );
}

// Test para el método ColisionesEjeY_2
TEST(GridTest, ColisionesEjeY_2) {
  int nx = 10;
  int ny = 10;
  int nz = 10;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.i = 0;
  p.j = 0;
  p.k = 0;
  particles.push_back(p);
  myGrid.blocks[myGrid.generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeY_2
  myGrid.ColisionesEjeY_2(particles);
  // Verificar que la partícula ha colisionado correctamente
  ASSERT_DOUBLE_EQ(particles[0].a_x, 1 );
}

// Test para el método ColisionesEjeZ_2
TEST(GridTest, ColisionesEjeZ_2) {
  int nx = 10;
  int ny = 10;
  int nz = 10;
  grid myGrid(nx, ny, nz);
  std::vector<Particle> particles;
  // Añadir una partícula al bloque (0,0,0)
  Particle p;
  p.i = 0; p.j = 0; p.k = 0;
  particles.push_back(p);
  myGrid.blocks[myGrid.generarClaveBloque(0, 0, 0)].addParticle(0);
  // Llamar a ColisionesEjeZ_2
  myGrid.ColisionesEjeZ_2(particles);
  // Verificar que la partícula ha colisionado correctamente
  ASSERT_DOUBLE_EQ(particles[0].a_x, 1 );
}*/

