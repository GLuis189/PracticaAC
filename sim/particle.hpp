//
// Created by belen on 4/11/23.
//

#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP
//#include "../sim/grid.hpp"


class Particle {
  public:
    int ide;
    double p_x, p_y, p_z;   // Coordenadas de posición
    double hvx, hvy, hvz; // Coordenadas del vector hv
    double v_x, v_y, v_z;   // Coordenadas de la velocidad
    double a_x, a_y, a_z;
    int i, j, k;
    double densidad;
    void calcularBloqueInicial(double s_x, double s_y, double s_z, int n_x, int n_y, int n_z);
    void calcularBloque(double s_x, double s_y, double s_z, int n_x, int n_y, int n_z);
    //void calcularDensidad(Particle& p_i, std::vector<Particle>& particles, int numparticulas, float suavizado);
    void Inicializar();
    void VariacionDensidad(Particle & particula, double suavizado_2);
    double CalcularDistancia(Particle & particula) const;
    void VariacionAcelaracionX(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa);
    void VariacionAcelaracionY(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa);
    void VariacionAcelaracionZ(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa);
    void ColisionesEjeX0_1();
    void ColisionesEjeXnx_1();
    void ColisionesEjeY0_1();
    void ColisionesEjeYny_1();
    void ColisionesEjeZ0_1();
    void ColisionesEjeZnz_1();

    void MoverParticulas(int n_x, int n_y, int n_z);

    void ColisionesEjeX_2();
    void ColisionesEjeY_2();
    void ColisionesEjeZ_2();
    bool operator==(const Particle& other) const;
    bool operator<(const Particle& other) const;
    bool operator>(const Particle& other) const;

};
#endif  // FLUID_PARTICLE_HPP
