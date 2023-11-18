#include "../sim/block.hpp"
#include "../sim/grid.hpp"
#include "../sim/particle.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

template <typename T>
typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, char*>
    as_writable_buffer(T& value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<char*>(&value);
}

template <typename T>
typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, const char*>
    as_buffer(const T& value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<const char*>(&value);
}

template <typename T>
typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, T>
    read_binary_value(std::istream& is) {
  T value{};
  is.read(as_writable_buffer(value), sizeof(value));
  return value;
}

template <typename... Args>
bool read_binary_values(std::ifstream& inputfile, Args&... args) {
  return (inputfile.read(as_writable_buffer(args), sizeof(args)) && ...);
}


int main(int argc, char *argv[]) {
  ProgArgs const args(argc, argv);

  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);

  grid malla = CalcularVariables(inputfile);

  std::vector<Particle> particles = leerparticulas(inputfile, malla);

  mostrardatos();
  // Inicio de simulación

  for (int time = 0; time < args.nts; time++) {
    if(time>0) {
      for (int part = 0; part < numparticulas; part++) {
        Particle & particle = particles[part];
        particle.densidad   = 0;
        particle.ax         = g_x;
        particle.ay         = g_y;
        particle.az         = g_z;

        int i_anterior = particle.i;
        int j_anterior = particle.j;
        int k_anterior = particle.k;

        particle.i = static_cast<int>((particle.px - bmin_x) / sx);

        if (particle.i > nx - 1) {
          particle.i = nx - 1;
        }
        if (particle.i < 0) { particle.i = 0; }
        particle.j = static_cast<int>((particle.py - bmin_y) / sy);
        if (particle.j > ny - 1) { particle.j = ny - 1; }
        if (particle.j < 0) { particle.j = 0; }
        particle.k = static_cast<int>((particle.pz - bmin_z) / sz);
        if (particle.k > nz - 1) { particle.k = nz - 1; }
        if (particle.k < 0) { particle.k = 0; }
        if (i_anterior != particle.i || j_anterior != particle.j || k_anterior != particle.k) {
          std::string block_key = std::to_string(i_anterior) + "_" + std::to_string(j_anterior) +
                                  "_" + std::to_string(k_anterior);
          malla.blocks[block_key].removeParticle(particle.id);
          std::string block_key2 = std::to_string(particle.i) + "_" + std::to_string(particle.j) +
                                   "_" + std::to_string(particle.k);
          malla.blocks[block_key2].addParticle(particle.id);
        }
      }
    }
    for (int part = 0; part < numparticulas; part++) {
      Particle & particle = particles[part];

      std::string block_key2 = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" +
                               std::to_string(particle.k);
      std::vector<std::string> bloquesAdy = malla.blocks[block_key2].bloques_ady;

      for (std::string& bloque : bloquesAdy) {
        std::vector<int>& particulas_ady = malla.blocks[bloque].particles;
        for (int id: particulas_ady) {
          Particle & particula = particles[id];
          if (particula > particle) {
            double p_dif_x = particle.px- particula.px;
            double p_dif_y = particle.py- particula.py;
            double p_dif_z = particle.pz- particula.pz;
            double distancia = std::sqrt(p_dif_x* p_dif_x +p_dif_y * p_dif_y +p_dif_z* p_dif_z);
            double variacion_densidad = 0;
            double distancia_2 = distancia*distancia;
            if((distancia_2) < (suavizado_2)) {
              variacion_densidad = (suavizado_2 - distancia_2)*(suavizado_2 - distancia_2)* (suavizado_2 - distancia_2);

              particle.densidad = particle.densidad + variacion_densidad;
              particula.densidad = particula.densidad + variacion_densidad;
            }
          }
        }
      }
      particle.densidad = (particle.densidad + pow(suavizado, 6)) * 315 * masa/ (64 * M_PI * pow(suavizado, 9));
    }
    for (int part = 0; part < numparticulas; part++) {
      Particle & particle = particles[part];
      std::string block_key2 = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" +
                               std::to_string(particle.k);
      std::vector<std::string> bloquesAdy = malla.blocks[block_key2].bloques_ady;
      for (std::string& bloque : bloquesAdy) {
        std::vector<int>& particulas_ady = malla.blocks[bloque].particles;
        for (int id: particulas_ady) {
          Particle & particula = particles[id];
          if (particula > particle) {
            double p_dif_x = particle.px- particula.px;
            double p_dif_y = particle.py- particula.py;
            double p_dif_z = particle.pz- particula.pz;
            double modulo = std::sqrt(p_dif_x* p_dif_x +p_dif_y * p_dif_y +p_dif_z* p_dif_z);
            double distancia = modulo*modulo;


            if(distancia< suavizado*suavizado) {
              double dist_ij = std::sqrt(std::max(distancia, 1e-12));

              double var_ax = ((((particle.px - particula.px)* (15/pi_sua_6) * ((3*masa * presion)/2) *
                               ((suavizado - dist_ij)*(suavizado - dist_ij))/dist_ij) * (particle.densidad + particula.densidad - 2*densidad)) +
                              ((particula.vx - particle.vx) * (45/pi_sua_6) * (vis * masa)))/(particle.densidad * particula.densidad) ;
              particle.ax = particle.ax + var_ax;
              particula.ax = particula.ax - var_ax;

              double var_ay = ((((particle.py - particula.py)* (15/pi_sua_6) * ((3*masa * presion)/2) *
                                 ((suavizado - dist_ij)*(suavizado - dist_ij))/dist_ij) * (particle.densidad + particula.densidad - 2*densidad)) +
                               ((particula.vy - particle.vy) * (45/pi_sua_6) * (vis * masa))) / (particle.densidad * particula.densidad);
              particle.ay = particle.ay + var_ay;
              particula.ay = particula.ay - var_ay;

              double var_az = ((((particle.pz - particula.pz)* (15/pi_sua_6) * ((3*masa * presion)/2) *
                                 ((suavizado - dist_ij)*(suavizado - dist_ij))/dist_ij) * (particle.densidad + particula.densidad - 2*densidad)) +
                               ((particula.vz - particle.vz) * (45/pi_sua_6) * (vis * masa))) / (particle.densidad * particula.densidad);
              particle.az = particle.az + var_az;
              particula.az = particula.az -var_az;
            }
          }
        }
      }
    }
    for (std::string& bloque: malla.colisionesCx){
      std::vector<int>& particulas_0x = malla.blocks[bloque].particles;
      for (int id : particulas_0x) {
        Particle & particula = particles[id];
        double x = particula.px + particula.hvx*ptiempo;
        double var_px;
          if (malla.blocks[bloque].i == 0){
          var_px = tparticula - (x - bmin_x);
        }
        else{
          var_px = tparticula - ( bmax_x - x);
        }
        if (var_px > 1e-10){
          if(malla.blocks[bloque].i == 0){
            particula.ax = particula.ax + (colisiones*var_px - amortiguamiento*particula.vx);
          }
          else{
            particula.ax = particula.ax - (colisiones*var_px + amortiguamiento*particula.vx);
          }
        }
      }

    }
    for (std::string& bloque: malla.colisionesCy){
      std::vector<int>& particulas_0y = malla.blocks[bloque].particles;
      for (int id : particulas_0y) {
        Particle & particula = particles[id];
        double y = particula.py + particula.hvy*ptiempo;
        double var_py;
        if (malla.blocks[bloque].j == 0){
          var_py = tparticula - (y- bmin_y);
        }
        else{
          var_py = tparticula - ( bmax_y - y);
        }
        if (var_py  > 1e-10){
          if(malla.blocks[bloque].j == 0){
            particula.ay = particula.ay + (colisiones*var_py- amortiguamiento*particula.vy);
          }
          else{
            particula.ay = particula.ay - (colisiones*var_py  + amortiguamiento*particula.vy);
          }
        }
      }

    }
    for (std::string& bloque: malla.colisionesCz){
      std::vector<int>& particulas_0z = malla.blocks[bloque].particles;
      for (int id : particulas_0z) {
        Particle & particula = particles[id];
        double z = particula.pz+ particula.hvz*ptiempo;
        double var_pz;
        if (malla.blocks[bloque].k == 0){
          var_pz = tparticula - (z - bmin_z);
        }
        else{
          var_pz = tparticula - ( bmax_z - z);
        }
        if (var_pz  > 1e-10){
          if(malla.blocks[bloque].k == 0){
            particula.az = particula.az + (colisiones*var_pz - amortiguamiento*particula.vz);
          }
          else{
            particula.az = particula.az - (colisiones*var_pz + amortiguamiento*particula.vz);
          }
        }
      }
    }
    for (int part = 0; part < numparticulas; part++){
      Particle & particle = particles[part];

      particle.px = particle.px + particle.hvx * ptiempo + particle.ax * (ptiempo * ptiempo);
      particle.py = particle.py + particle.hvy * ptiempo + particle.ay * (ptiempo * ptiempo);
      particle.pz = particle.pz + particle.hvz * ptiempo + particle.az * (ptiempo * ptiempo);

      particle.vx = particle.hvx + (particle.ax * ptiempo) / 2;
      particle.vy = particle.hvy + (particle.ay * ptiempo) / 2;
      particle.vz = particle.hvz + (particle.az * ptiempo) / 2;

      particle.hvx = particle.hvx + particle.ax * ptiempo;
      particle.hvy = particle.hvy + particle.ay * ptiempo;
      particle.hvz = particle.hvz + particle.az * ptiempo;

    }

    for (std::string& bloque: malla.colisionesCx) {
      std::vector<int>& particulas_0x = malla.blocks[bloque].particles;
      for (int id : particulas_0x) {
        Particle & particula = particles[id];
        double d_x;
        if (malla.blocks[bloque].i == 0){
          d_x = particula.px - bmin_x;
        }
        else{
          d_x = bmax_x - particula.px;
        }
        if(d_x<0){
          if (malla.blocks[bloque].i == 0){
            particula.px = bmin_x - d_x;
          }
          else{
            particula.px = bmax_x + d_x;
          }
          particula.vx = -particula.vx;
          particula.hvx = -particula.hvx;
        }
      }
    }

    for (std::string& bloque: malla.colisionesCy) {
      std::vector<int>& particulas_0y = malla.blocks[bloque].particles;
      for (int id : particulas_0y) {
        Particle & particula = particles[id];
        double d_y;
        if (malla.blocks[bloque].j == 0){
          d_y = particula.py - bmin_y;
        }
        else{
          d_y = bmax_y - particula.py;

        }
        if(d_y<0){
          if (malla.blocks[bloque].j == 0){
            particula.py = bmin_y - d_y;
          }
          else{
            particula.py = bmax_y + d_y;
          }
          particula.vy = -particula.vy;
          particula.hvy = -particula.hvy;
        }
      }
    }
    for (std::string& bloque: malla.colisionesCz) {
      std::vector<int>& particulas_0z = malla.blocks[bloque].particles;
      for (int id : particulas_0z) {
        Particle & particula = particles[id];
        double d_z;
        if (malla.blocks[bloque].k == 0){
          d_z = particula.pz - bmin_z;
        }
        else{
          d_z = bmax_z - particula.pz;
        }
        if(d_z<0){
          if (malla.blocks[bloque].k == 0){
            particula.pz = bmin_z - d_z;
          }
          else{
            particula.pz = bmax_z + d_z;
          }
          particula.vz = -particula.vz;
          particula.hvz = -particula.hvz;
        }
      }
    }
  }
  // Mostrar los datos de las partículas
  for (Particle & particle: particles) {
    outputfile << "ID: " << particle.id << "\n";
    outputfile << "Posición (x, y, z): " << particle.px << ", " << particle.py << ", " << particle.pz << "\n";
    outputfile << "Velocidad (vx, vy, vz): " << particle.vx << ", " << particle.vy << ", " << particle.vz << "\n";
    outputfile << "Hvx, Hvy, Hvz: " << particle.hvx << ", " << particle.hvy << ", " << particle.hvz << "\n";
    outputfile << "Densidad: " << particle.densidad << "\n";
    outputfile << "Aceleración (accx, accy, accz): " << particle.ax << ", " << particle.ay << ", " << particle.az << "\n";
  }
  inputfile.close();
  return 0;
}