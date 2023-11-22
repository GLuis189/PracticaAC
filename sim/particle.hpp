
#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP
#include <vector>
#include <stdexcept>

struct Vector{
    double c_x, c_y, c_z;
    double& operator[] (int i) {
      if(i == 0) { return c_x; };
      if(i == 1) { return c_y; };
      if(i == 2) { return c_z; };
      throw std::out_of_range("Index out of range");
    }
};

class Particle {
  public:
    int ide{};
    Vector posicion{};   // Coordenadas de posición
    Vector hvelocidad{}; // Coordenadas del vector hv
    Vector velocidad{};   // Coordenadas de la velocidad
    Vector aceleracion{};
    int p_i{}, p_j{}, p_k{};
    std::vector<int> particulas_adyacentes{};
    double densidad{};
    void Inicializar();
    void VariacionDensidad(Particle & particula, double suavizado_2);
    double CalcularDistancia(Particle & particula) const;
    void VariacionAcelaracion(Particle & particula, double suavizado, double pi_sua_6, double masa);
    void ColisionesEje(int eje, double bmax, double bmin);

    void MoverParticulas(int n_x, int n_y, int n_z);

    void ColisionesEje_2(int eje, double bmax, double bmin);
    bool operator==(const Particle& other) const;
    bool operator<(const Particle& other) const;
    bool operator>(const Particle& other) const;

};
#endif  // FLUID_PARTICLE_HPP
