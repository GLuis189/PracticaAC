//
// Created by luis on 30/10/23.
//
// Block.cpp
#include "block.hpp"
#include <algorithm>


void Block::addParticle(int p) {
  particles.push_back(p);
}

void Block::removeParticle(int p){
  //auto pos = std::find(particles.begin(), particles.end(), p);
  particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end());
}
/*void Block::printParticles() {
  std::cout << "Particles in the block:" << std::endl;
  for (const auto& particle : particles) {
    std::cout << "Hay particulas " << std::endl;
    std::cout << "Position: (" <<  particle.id<< ", " << particle.px << ", " << particle.py << ", " << particle.pz << ")" << std::endl;
    // Puedes imprimir otras propiedades de la partícula si lo deseas
    // Ejemplo: velocidad, índices i, j, k, etc.
  }
  std::cout << "End of particles" << std::endl;
}
void Block::printBlockAndAdyacentes() {
  // Imprime el bloque actual
  std::cout << "Bloque (" << i << ", " << j << ", " << k << "):" << std::endl;
  printParticles();

  // Imprime la lista de bloques adyacentes
  std::cout << "Bloques adyacentes:" << std::endl;
  for (const auto& adyacente : bloques_ady) {
    //std::cout << "Hay bloques adyacentes:" << std::endl;
    std::cout << adyacente << std::endl;
  }
  std::cout << std::endl;
}*/



