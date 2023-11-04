//
// Created by luis on 30/10/23.
//

/*#ifndef PRACTICAAC_BLOCK_HPP
#define PRACTICAAC_BLOCK_HPP



class block {
  public:
    float sx;
    float sy;
    float sz;

    block(float bmax_x, float bmax_y, float bmax_z, float bmin_x, float bmin_y, float bmin_z, grid malla);
};

#endif //PRACTICAAC_BLOCK_HPP*/
// Block.hpp
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include "particle.hpp"

class Block {
  private:
    std::vector<Particle> particles;

  public:
    void addParticle(const Particle& p);
    // Otros métodos para trabajar con las partículas en el bloque si es necesario
};

#endif // BLOCK_HPP

