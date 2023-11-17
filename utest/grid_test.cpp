//
// Created by luis on 30/10/23.
//
// test_grid.cpp
#include "../gtest/gtest.h"
#include "../sim/grid.hpp"

TEST(GridTest, BlockCreation) {
  int nx = 3;
  int ny = 3;
  int nz = 3;
  grid myGrid(nx, ny, nz);

  // Verificar que se crearon todos los bloques
  EXPECT_EQ(nx * ny * nz, myGrid.blocks.size());

  // Verificar que cada bloque tiene las coordenadas correctas
  for (const auto& pair : myGrid.blocks) {
        const Block& block = pair.second;
    EXPECT_GE(block.i, 0);
    EXPECT_LT(block.i, nx);
    EXPECT_GE(block.j, 0);
    EXPECT_LT(block.j, ny);
    EXPECT_GE(block.k, 0);
    EXPECT_LT(block.k, nz);
  }
}
