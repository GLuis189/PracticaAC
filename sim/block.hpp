//
// Created by luis on 30/10/23.
//

// Block.hpp
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include "particle.hpp"
#include <iostream>

class Block {
  public:
    int i, j ,k;
    void addParticle(int parti);
    void removeParticle(int  parti);
    // void printBlockAndAdyacentes();
    //void printParticles();  // Nuevo método para imprimir las partículas
    std::vector<int> particles;
    std::vector<int> bloques_ady;
};

#endif // BLOCK_HPP

