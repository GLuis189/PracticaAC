//
// Created by luis on 30/10/23.
//

/*#ifndef PRACTICAAC_GRID_HPP
#define PRACTICAAC_GRID_HPP


class grid {
  public:
    int nx;
    int ny;
    int nz;

    grid(float bmax_x, float bmax_y, float bmax_z, float bmin_x, float bmin_y, float bmin_z, float suavizado);
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
    //void printBlocks();

    // Otros métodos relacionados con la malla
};

#endif // MESH_HPP


