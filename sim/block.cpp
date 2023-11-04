//
// Created by luis on 30/10/23.
//
/*#include "grid.hpp"
#include "block.hpp"
block::block(float bmax_x, float bmax_y, float bmax_z, float bmin_x, float bmin_y, float bmin_z, grid grid) {
  sx = (bmax_x - bmin_x)/grid.nx;
  sy = (bmax_y - bmin_y)/grid.ny;
  sz = (bmax_z - bmin_z)/grid.nz;


}*/
// Block.cpp
#include "block.hpp"

void Block::addParticle(const Particle& p) {
  particles.push_back(p);
  // Lógica adicional de añadir partículas al bloque si es necesario
}
