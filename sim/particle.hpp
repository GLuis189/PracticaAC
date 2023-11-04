//
// Created by belen on 4/11/23.
//

#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP
struct Particle {
    double px, py, pz;   // Coordenadas de posición
    double hvx, hvy, hvz; // Coordenadas del vector hv
    double vx, vy, vz;   // Coordenadas de la velocidad
    int i, j, k;
};
#endif  // FLUID_PARTICLE_HPP
