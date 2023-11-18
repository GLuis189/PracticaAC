//
// Created by Celia on 18/11/23.
//
#include "gtest/gtest.h"
#include "../sim/particle.hpp"

TEST(ParticleTest, CalcularBloque) {
  Particle particle;
  particle.px = 10;
  particle.py = 20;
  particle.pz = 30;

  particle.calcularBloque(particle, 0, 5, 0, 5, 0, 5, 10, 10, 10);

  EXPECT_EQ(particle.i, 2);
  EXPECT_EQ(particle.j, 4);
  EXPECT_EQ(particle.k, 6);
}
