#include "../sim/grid.hpp"
#include "../sim/particle.hpp"
#include "../sim/progargs.hpp"
#include "../sim/variablesglobales.hpp"
#include "../sim/simulacion.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

double ppm;
int numparticulas;
double masa;
double suavizado;
double pi_sua_6;
double suavizado_2;
int nx ;
int ny ;
int nz ;
double sx;
double sy;
double sz;

grid CalcularVariables(std::ifstream& inputfile) {
  ppm           = read_binary_value<float>(inputfile);
  numparticulas = read_binary_value<int>(inputfile);

  if (numparticulas <= 0) {
    std::cerr << "Error: Invalid number of particles: 0.\n";
    exit(-5);
  }

  masa      = densidad / (ppm * ppm * ppm);
  suavizado = radio / ppm;
  pi_sua_6 = M_PI * suavizado * suavizado * suavizado * suavizado * suavizado * suavizado;
  suavizado_2 = suavizado * suavizado;

  nx    = static_cast<int>((bmax_x - bmin_x) / suavizado);
  ny    = static_cast<int>((bmax_y - bmin_y) / suavizado);
  nz    = static_cast<int>((bmax_z - bmin_z) / suavizado);
  sx = (bmax_x - bmin_x) / nx;
  sy = (bmax_y - bmin_y) / ny;
  sz = (bmax_z - bmin_z) / nz;
  grid malla(nx, ny, nz);
  return malla;
}

std::vector<Particle> leerparticulas(std::ifstream& inputfile, grid & malla) {
  std::vector<Particle> particles;
  particles.reserve(numparticulas);
  int contar_particulas = 0;
  float px, py, pz, hvx, hvy, hvz, vx, vy, vz;
  while (read_binary_values(inputfile, px, py, pz, hvx, hvy, hvz, vx, vy, vz)) {
    Particle particle;
    particle.id = contar_particulas, particle.densidad = 0;
    particle.px = px, particle.py = py, particle.pz = pz;
    particle.hvx = px, particle.hvy = py, particle.hvz = pz;
    particle.vx = vx, particle.vy = vy, particle.vz = vz;
    particle.ay = -9.8, particle.ax = 0, particle.az =0;
    Particle::calcularBloque(particle, bmin_x, sx, bmin_y, sy, bmin_z, sz, nx, ny, nz);
    std::string block_key = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" +
                            std::to_string(particle.k);
    malla.blocks[block_key].addParticle(particle.id);
    particles.push_back(particle);
    ++contar_particulas;
  }
  inputfile.close();
  if (numparticulas != contar_particulas) {
    std::cerr << "Error: Number of particles mismatch. Header:" << numparticulas
              << " Found:" << contar_particulas << "\n";
    exit(-5);
  }
 return particles;
}


void IniciarSimulacion(ProgArgs args, std::ofstream& outputfile, grid & malla, std::vector<Particle>& particles){
  for (int time = 0; time < args.nts; time++) {
    if(time>0) {
      for (int part = 0; part < numparticulas; part++) {
        Particle & particle = particles[part];
        particle.densidad   = 0;
        particle.ax         = 0;
        particle.ay         = -9.8;
        particle.az         = 0;

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
  outputfile.close();
}

void mostrardatos(){
  // Mostrar los datos
  std::cout << "Number of particles: " << numparticulas << "\n";
  std::cout << "Particles per meter: " << ppm << "\n";
  std::cout << "Smoothing length: " << suavizado << "\n";
  std::cout << "Particle mass: " << masa << "\n";
  std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << "\n";
  std::cout << "Number of blocks: " << nx * ny * nz << "\n";
  std::cout << "Block size: " << sx << " x " << sy << " x " << sz << "\n";
}