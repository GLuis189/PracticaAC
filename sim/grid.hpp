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

#include <string>
#include <unordered_map>

class grid {
  public:
    std::unordered_map<std::string, Block> blocks; // Almacena los bloques
    std::vector<std::string> colisionesCx;
    std::vector<std::string> colisionesCy;
    std::vector<std::string> colisionesCz;

    grid(int nx, int ny, int nz);
    void CambiarBloque(int id, int i, int j, int k, int i_anterior, int j_anterior, int k_anterior);
    void calcularDensidades(std::vector<Particle> & particles, int numparticulas, double masa, double suavizado, double suavizado_2);
    void calcularBloquesAdyacentes(const std::string& block_key, int a, int b, int c, int nx, int ny, int nz);
    std::string generarClaveBloque(int a, int b, int c);
    void calcularAceleraciones(std::vector<Particle> & particles, int numparticulas, double pi_sua_6, double masa, double suavizado);
    void ColisionesEjeX_1(std::vector<Particle> & particles);
    void ColisionesEjeY_1(std::vector<Particle> & particles);
    void ColisionesEjeZ_1(std::vector<Particle> & particles);

    //void printBlocks();

    // Otros métodos relacionados con la malla
};

#endif // MESH_HPP


