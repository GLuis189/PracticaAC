//
// Created by luis on 30/10/23.
//

/*#ifndef PRACTICAAC_GRID_HPP
#define PRACTICAAC_GRID_HPP


class grid {

};


#endif //PRACTICAAC_GRID_HPP*/
// Mesh.hpp
#ifndef MESH_HPP
#define MESH_HPP

#include "block.hpp"
#include <unordered_map>

class grid {
  public:
    std::unordered_map<std::string, Block> blocks; // Almacena los bloques

    grid(int nx, int ny, int nz);
    void printBlocks();

    // Otros métodos relacionados con la malla
};

#endif // MESH_HPP


