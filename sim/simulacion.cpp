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

grid calcularMalla(std::ifstream& inputfile) {
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

std::vector<Particle> leerParticulas(std::ifstream& inputfile, grid & malla) {
  std::vector<Particle> particles;
  particles.reserve(numparticulas);
  int contar_particulas = 0;
  float px, py, pz, hvx, hvy, hvz, vx, vy, vz;
  while(read_binary_values(inputfile,  px, py, pz, hvx, hvy, hvz, vx, vy, vz)) {
    Particle particle;
    particle.id = contar_particulas, particle.densidad = 0;
    particle.px = px, particle.py = py, particle.pz = pz;
    particle.hvx = hvx, particle.hvy = hvy, particle.hvz = hvz;
    particle.vx = vx, particle.vy = vy, particle.vz = vz;
    particle.ay = -9.8, particle.ax = 0, particle.az = 0;
    particle.calcularBloqueInicial(sx, sy, sz, nx, ny, nz);
    std::string const block_key = malla.generarClaveBloque(particle.i, particle.j, particle.k);
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

void reposicionar(grid & malla, std::vector<Particle> & particles) {
  for (int part = 0; part < numparticulas; part++) {
    Particle & particle = particles[part];

    particle.Inicializar();

    int i_anterior = particle.i;
    int j_anterior = particle.j;
    int k_anterior = particle.k;

    particle.calcularBloque(sx, sy, sz, nx, ny, nz);

    if (i_anterior != particle.i || j_anterior != particle.j || k_anterior != particle.k) {
      malla.CambiarBloque(particle.id, particle.i, particle.j, particle.k, i_anterior, j_anterior, k_anterior);
    }
  }
}

void IniciarSimulacion(const ProgArgs& args, std::ofstream& outputfile, grid & malla, std::vector<Particle>& particles){
  for (int time = 0; time < args.nts; time++) {
    if(time>0) {
     reposicionar(malla, particles);
    }
    malla.calcularDensidades(particles, numparticulas, masa, suavizado, suavizado_2);

    malla.calcularAceleraciones(particles, numparticulas, pi_sua_6, masa, suavizado);

    malla.ColisionesEjeX_1(particles);
    malla.ColisionesEjeY_1(particles);
    malla.ColisionesEjeZ_1(particles);

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

void mostrarDatos(){
  // Mostrar los datos
  std::cout << "Number of particles: " << numparticulas << "\n";
  std::cout << "Particles per meter: " << ppm << "\n";
  std::cout << "Smoothing length: " << suavizado << "\n";
  std::cout << "Particle mass: " << masa << "\n";
  std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << "\n";
  std::cout << "Number of blocks: " << nx * ny * nz << "\n";
  std::cout << "Block size: " << sx << " x " << sy << " x " << sz << "\n";
}