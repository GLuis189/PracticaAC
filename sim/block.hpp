// Block.hpp
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include "particle.hpp"
#include <iostream>
#include <algorithm>

class Block {
  public:
    void addParticle(int parti);
    void removeParticle(int  parti);
    std::vector<int> particles;
    std::vector<int> bloques_ady;
};

#endif // BLOCK_HPP

