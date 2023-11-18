//
// Created by belen on 4/11/23.
//

#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP
//#include "../sim/grid.hpp"

class Particle {
  public:
    int id;
    double px, py, pz;   // Coordenadas de posición
    double hvx, hvy, hvz; // Coordenadas del vector hv
    double vx, vy, vz;   // Coordenadas de la velocidad
    double ax, ay, az;
    int i, j, k;
    double densidad;
    void calcularBloqueInicial(double sx, double sy, double sz, int nx, int ny, int nz);
    void calcularBloque(double sx, double sy, double sz, int nx, int ny, int nz);
    //void calcularDensidad(Particle& p_i, std::vector<Particle>& particles, int numparticulas, float suavizado);
    void Inicializar();
    void VariacionDensidad(Particle & particula, double suavizado_2);
    double CalcularDistancia(Particle & particula);
    double VariacionAcelaracionX(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa);
    void VariacionAcelaracionY(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa);
    void VariacionAcelaracionZ(Particle & particula, double suavizado, double pi_sua_6, double dist_ij, double masa);
    void ColisionesEjeX_1();
    void ColisionesEjeY_1();
    void ColisionesEjeZ_1();

    void MoverParticulas();

    void ColisionesEjeX_2();
    void ColisionesEjeY_2();
    void ColisionesEjeZ_2();
    bool operator==(const Particle& other) const;
    bool operator<(const Particle& other) const;
    bool operator>(const Particle& other) const;
};
#endif  // FLUID_PARTICLE_HPP
