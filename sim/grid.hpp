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

    int n_x; int n_y; int n_z;
    double s_x; double s_y; double s_z;

    grid(double suavizado);
    void calcularBloque(Particle & particle) const;
    void calcularBloqueInicial(Particle & particle) const;
    void CambiarBloque(Particle & particle, int i_anterior, int j_anterior, int k_anterior);
    void calcularDensidades(std::vector<Particle> & particles, double masa, double suavizado, double suavizado_2);
    static bool esValido(int indice, int max);
    void calcularBloquesAdyacentes(int block_key, int var_a, int var_b, int var_c);
    static int generarClaveBloque(int var_a, int var_b, int var_c);
    static void calcularAceleraciones(std::vector<Particle> & particles, double pi_sua_6, double masa, double suavizado);


    // Otros métodos relacionados con la malla
};

#endif // MESH_HPP


