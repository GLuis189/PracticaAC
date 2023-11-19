//
// Created by luis on 30/10/23.
//
//
#include "../sim/block.hpp"
#include "gtest/gtest.h"

TEST(BlockTest, addParticle) {
  Block block;
  block.addParticle(1);

  EXPECT_EQ(block.particles.size(), 1);
  EXPECT_EQ(block.particles[0], 1);
}

TEST(BlockTest, removeParticle) {
  Block block;
  block.addParticle(1);
  block.addParticle(2);
  block.addParticle(3);

  block.removeParticle(2);

  EXPECT_EQ(block.particles.size(), 2);
  EXPECT_EQ(block.particles[0], 1);
  EXPECT_EQ(block.particles[1], 3);
}
