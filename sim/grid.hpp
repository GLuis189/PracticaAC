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
    std::unordered_map<int, Block> blocks; // Almacena los bloques
    std::vector<int> colisionesCx;
    std::vector<int> colisionesCy;
    std::vector<int> colisionesCz;

    grid(int n_x, int n_y, int n_z);
    void CambiarBloque(int ide, int b_i, int b_j, int b_k, int i_anterior, int j_anterior, int k_anterior);
    void calcularDensidades(std::vector<Particle> & particles, double masa, double suavizado, double suavizado_2);
    static bool esValido(int indice, int max);
    void calcularBloquesAdyacentes(const int block_key, int vara, int varb, int varc, int n_x, int n_y, int n_z);
    static int generarClaveBloque(int vara, int varb, int varc);
    void calcularAceleraciones(std::vector<Particle> & particles, double pi_sua_6, double masa, double suavizado);
    void ColisionesEjeX_1(std::vector<Particle> & particles);
    void ColisionesEjeY_1(std::vector<Particle> & particles);
    void ColisionesEjeZ_1(std::vector<Particle> & particles);
    void ColisionesEjeX_2(std::vector<Particle> & particles);
    void ColisionesEjeY_2(std::vector<Particle> & particles);
    void ColisionesEjeZ_2(std::vector<Particle> & particles);

    //void printBlocks();

    // Otros métodos relacionados con la malla
};

#endif // MESH_HPP


