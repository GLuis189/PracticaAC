//
// Created by luis on 30/10/23.
//


/*#include <iostream>
#include <fstream>
#include <cstdlib>
#include "grid.hpp"

grid::grid(float bmax_x, float bmax_y, float bmax_z, float bmin_x, float bmin_y, float bmin_z, float suavizado) {
  nx = static_cast<int>((bmax_x - bmin_x)/suavizado);
  ny = static_cast<int>((bmax_y - bmin_y)/suavizado);
  nz = static_cast<int>((bmax_z - bmin_z)/suavizado);

}*/
// Mesh.cpp
#include <iostream>
#include "grid.hpp"



grid::grid(int nx, int ny, int nz) {
  // Generar bloques para toda la malla
  for (int a = 0; a < nx; ++a) {
    for (int b = 0; b < ny; ++b) {
      for (int c = 0; c < nz; ++c) {
        std::string block_key = std::to_string(a) + "_" + std::to_string(b) + "_" + std::to_string(c);
        Block newBlock;
        blocks[block_key] = newBlock;
      }
    }
  }
}
