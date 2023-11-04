//
// Created by luis on 30/10/23.
//

/*#ifndef PRACTICAAC_BLOCK_HPP
#define PRACTICAAC_BLOCK_HPP


class block {

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

