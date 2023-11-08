//
// Created by belen on 4/11/23.
//

#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP
class Particle {
  public:
    int id;
    double px, py, pz;   // Coordenadas de posición
    double hvx, hvy, hvz; // Coordenadas del vector hv
    double vx, vy, vz;   // Coordenadas de la velocidad
    int i, j, k;
    double densidad;
    static void calcularBloque(Particle& p, float bmin_x, float sx, float bmin_y, float sy, float bmin_z, float sz , int nx, int ny, int nz);
    bool operator==(const Particle& other) const;
};
#endif  // FLUID_PARTICLE_HPP
