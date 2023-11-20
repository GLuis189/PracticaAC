//
// Created by luis on 30/10/23.
//
// test_grid.cpp
#include "gtest/gtest.h"
#include "../sim/grid.hpp"
#include "../sim/variablesglobales.hpp"

TEST(GridTest, BlockCreation) {
  int const n_x = 3;
  int const n_y = 3;
  int const nz = 3;
  grid const myGrid(n_x, n_y, nz);

  // Verificar que se crearon todos los bloques
  EXPECT_EQ(n_x * n_y * nz, myGrid.blocks.size());

  // Verificar que cada bloque tiene las coordenadas correctas
  for (const auto& pair : myGrid.blocks) {
        const Block& block = pair.second;
    EXPECT_GE(block.i, 0);
    EXPECT_LT(block.i, n_x);
    EXPECT_GE(block.j, 0);
    EXPECT_LT(block.j, n_y);
    EXPECT_GE(block.k, 0);
    EXPECT_LT(block.k, nz);
  }
}

TEST(GridTest, generarClaveBloque) {
  int const n_x = 5;
  int const n_y = 5;
  int const n_z = 5;
  grid const myGrid(n_x, n_y, n_z);
  // Verificar que este método genera la clave correcta para un bloque dado sus índices
  ASSERT_EQ(myGrid.generarClaveBloque(1, 2, 3), "1_2_3");
}

// Test para el método esValido
TEST(GridTest, esValido) {
  int const n_x = 10;
  int const n_y = 10;
  int const n_z = 10;
  grid myGrid(n_x, n_y, n_z);
  // Verificar que este método devuelve true para índices válidos y false para índices no válidos
  ASSERT_TRUE(myGrid.esValido(5, 10));
  ASSERT_FALSE(myGrid.esValido(10, 10));
}

// Test para el método calcularBloquesAdyacentes
TEST(GridTest, calcularBloquesAdyacentes) {
  int nx = 10;
  int ny = 10;
  int nz = 10;
  grid myGrid(nx, ny, nz);
  // Verificar que este método calcula correctamente los bloques adyacentes para un bloque dado
  std::string const block_key2 = myGrid.generarClaveBloque(5, 5, 5);
  myGrid.calcularBloquesAdyacentes(block_key2, 5, 5, 5, 10, 10, 10);
  ASSERT_EQ(myGrid.blocks[block_key2].bloques_ady.size(), 27);
}

// Test para el método CambiarBloque
TEST(GridTest, CambiarBloque) {
  int nx = 10;
  int ny = 10;
  int nz = 10;
  grid myGrid(nx, ny, nz);
  // Añadir una partícula al bloque (5,5,5)
  std::string const block_key = myGrid.generarClaveBloque(5, 5, 5);
  myGrid.blocks[block_key].addParticle(1);
  // Cambiar la partícula al bloque (6,6,6)
  myGrid.CambiarBloque(1, 6, 6, 6, 5, 5, 5);
  // Verificar que la partícula se ha movido correctamente
  std::string const block_key2 = myGrid.generarClaveBloque(6, 6, 6);
  ASSERT_EQ(myGrid.blocks[block_key2].particles.size(), 1);
  ASSERT_EQ(myGrid.blocks[block_key].particles.size(), 0);
}

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
  ASSERT_DOUBLE_EQ(particles[0].a_x, 1/* valor esperado */);
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
  ASSERT_DOUBLE_EQ(particles[0].a_x, 1 /* valor esperado */);
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
  ASSERT_DOUBLE_EQ(particles[0].a_x, 1/* valor esperado */);
}