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
#include <algorithm>
#include <iostream>


void Block::addParticle(Particle& p) {
  particles.push_back(p);
  // Lógica adicional de añadir partículas al bloque si es necesario
}

void Block::removeParticle(Particle& p){
  auto pos = std::find(particles.begin(), particles.end(), p);
  particles.erase(pos);
}
void Block::printParticles() {
  std::cout << "Particles in the block:" << std::endl;
  for (const auto& particle : particles) {
    std::cout << "Position: (" <<  particle.id<< ", " << particle.px << ", " << particle.py << ", " << particle.pz << ")" << std::endl;
    // Puedes imprimir otras propiedades de la partícula si lo deseas
    // Ejemplo: velocidad, índices i, j, k, etc.
  }
  std::cout << "End of particles" << std::endl;
}


