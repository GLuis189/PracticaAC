//
// Created by luis on 30/10/23.
//
// Block.cpp
#include "block.hpp"



void Block::addParticle(int parti) {
  particles.emplace_back(parti);
}

void Block::removeParticle(int parti){
  auto pos = std::remove(particles.begin(), particles.end(), parti);
  particles.erase(pos, particles.end());
}
