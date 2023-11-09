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
        newBlock.i = a;
        newBlock.j = b;
        newBlock.k = c;
        blocks[block_key] = newBlock;
        if(a==0|| a==(nx-1)){
          colisionesCx.push_back(block_key);
        }
        if(b==0|| b==(ny-1)){
          colisionesCy.push_back(block_key);
        }
        if(c==0||c==(nz-1)){
          colisionesCz.push_back(block_key);
        }
        for (int i = -1; i < 2; i++){
          for (int j = -1; i < 2; i++){
            for (int k = -1; i < 2; i++){
              if (a+i>=0 && a+i<nx && b+j>=0 && b+j<ny && c+k>=0 && c+k<nz){
                std::string block_key2 = std::to_string(a + i) + "_" + std::to_string(b + j) + "_" + std::to_string(c + k);
                blocks[block_key].bloques_ady.push_back(block_key2);
              }
            }
          }
        }
      }
    }
  }
}

