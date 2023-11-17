#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../sim/progargs.hpp"
#include "../sim/grid.hpp"
#include "../sim/block.hpp"
#include "../sim/particle.hpp"
#include <algorithm>
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


int main(int argc, char *argv[]) {
  ProgArgs args(argc, argv);

  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);

  const double ppm             = read_binary_value<float>(inputfile);
  const int numparticulas     = read_binary_value<int>(inputfile);
  const double radio           = 1.695;
  const double densidad        = 1e3;
  const double presion         = 3.0;
  const double colisiones      = 3e4;
  const double amortiguamiento = 128.0;
  const double vis             = 0.4;  // Asume un valor para μ_p
  const double tparticula      = 2e-4;
  const double ptiempo         = 1e-3;  // Asume un valor para Δt

  const double g_x = 0.0;
  const double g_y = -9.8;
  const double g_z = 0.0;

  const double bmax_x = 0.065;
  const double bmax_y = 0.1;
  const double bmax_z = 0.065;

  const double bmin_x = -0.065;
  const double bmin_y = -0.08;
  const double bmin_z = -0.065;

  const double masa = densidad / (ppm*ppm*ppm);


  if (numparticulas <= 0) {
    std::cerr << "Error: Invalid number of particles: 0.\n";
    exit(-5);
  }

  const double suavizado = radio / ppm;
  const double pi_sua_6 = M_PI*suavizado*suavizado*suavizado*suavizado*suavizado*suavizado;

  int nx                = static_cast<int>((bmax_x - bmin_x) / suavizado);
  int ny                = static_cast<int>((bmax_y - bmin_y) / suavizado);
  int nz                = static_cast<int>((bmax_z - bmin_z) / suavizado);
  grid malla(nx, ny, nz);

  double sx = (bmax_x - bmin_x) / nx;
  double sy = (bmax_y - bmin_y) / ny;
  double sz = (bmax_z - bmin_z) / nz;

  // Lectura de la información de cada partícula
  std::vector<Particle> particles;
  particles.reserve(numparticulas);
  int contar_particulas = 0;

  while (!inputfile.eof()) {
    Particle particle;
    particle.px = read_binary_value<float>(inputfile);
    if (inputfile.eof()) {  // Verificar si se alcanzó el final del archivo después de la lectura
      break;                // Si se alcanza el final del archivo, salir del bucle
    }
    particle.id       = contar_particulas;
    particle.py       = read_binary_value<float>(inputfile);
    particle.pz       = read_binary_value<float>(inputfile);
    particle.hvx      = read_binary_value<float>(inputfile);
    particle.hvy      = read_binary_value<float>(inputfile);
    particle.hvz      = read_binary_value<float>(inputfile);
    particle.vx       = read_binary_value<float>(inputfile);
    particle.vy       = read_binary_value<float>(inputfile);
    particle.vz       = read_binary_value<float>(inputfile);
    particle.densidad = 0;
    particle.ay = -9.8;

    Particle::calcularBloque(particle, bmin_x, sx, bmin_y, sy, bmin_z, sz, nx, ny, nz);

    std::string block_key = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" +
                            std::to_string(particle.k);
    malla.blocks[block_key].addParticle(particle.id);
    particles.push_back(particle);
    ++contar_particulas;
  }

  if (numparticulas != contar_particulas) {
    std::cerr << "Error: Number of particles mismatch. Header:" << numparticulas
              << " Found:" << contar_particulas << "\n";
    exit(-5);
  }

  // Mostrar los datos
  std::cout << "Number of particles: " << numparticulas << "\n";
  std::cout << "Particles per meter: " << ppm << "\n";
  std::cout << "Smoothing length: " << suavizado << "\n";
  std::cout << "Particle mass: " << masa << "\n";
  std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << "\n";
  std::cout << "Number of blocks: " << nx * ny * nz << "\n";
  std::cout << "Block size: " << sx << " x " << sy << " x " << sz << "\n";

  /*for (int a = 0; a < nx; ++a) {
    for (int b = 0; b < ny; ++b) {
      for (int c = 0; c < nz; ++c) {
        std::string block_key =
            std::to_string(a) + "_" + std::to_string(b) + "_" + std::to_string(c);
        malla.blocks[block_key].printBlockAndAdyacentes();
      }
    }
  }*/

  // Inicio de simulación

  for (int time = 0; time < args.nts; time++) {
    for (int part = 0; part < numparticulas; part++) {
      Particle & particle = particles[part];
      particle.densidad = 0;
      particle.ax = g_x;
      particle.ay = g_y;
      particle.az = g_z;

      int i_anterior = particle.i;
      int j_anterior = particle.j;
      int k_anterior = particle.k;

      particle.i = static_cast<int>((particle.px - bmin_x) / sx);

      if (particle.i > nx - 1) {
        particle.i = nx - 1;
      }
      if(particle.i<0){
        particle.i=0;
      }
      particle.j = static_cast<int>((particle.py - bmin_y) / sy);
      if (particle.j > ny - 1) {
        particle.j = ny - 1;
      }
      if(particle.j<0){
        particle.j=0;
      }
      particle.k = static_cast<int>((particle.pz - bmin_z) / sz);
      if (particle.k > nz - 1) {
        particle.k = nz - 1;
      }
      if(particle.k<0){
        particle.k=0;
      }
      if (i_anterior != particle.i || j_anterior != particle.j || k_anterior != particle.k) {
        std::string block_key = std::to_string(i_anterior) + "_" + std::to_string(j_anterior) +
                                "_" + std::to_string(k_anterior);
        malla.blocks[block_key].removeParticle(particle.id);
        std::string block_key2 = std::to_string(particle.i) + "_" + std::to_string(particle.j) +
                                 "_" + std::to_string(particle.k);
        malla.blocks[block_key2].addParticle(particle.id);
      }
    }
    for (int part = 0; part < numparticulas; part++) {
      Particle & particle = particles[part];
      /*if(particle.id==1120){
        std::cout<<"--------------------------------------------\n";
        std::cout<<"Antes del bucle. Densidad" << particle.densidad;
        std::cout<<"Antes del bucle. X: "<<particle.px<<"Y: "<<particle.py<<"z: "<<particle.pz<<"\n";
        std::cout<<"Antes del bucle. VX: "<<particle.vx<<"vY: "<<particle.vy<<"vz: "<<particle.vz<<"\n";
        std::cout<<"Antes del bucle. aX: "<<particle.ax<<"aY: "<<particle.ay<<"az: "<<particle.az<<"\n";
        std::cout<<"--------------------------------------------\n";
      }*/


      std::string block_key2 = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" +
                               std::to_string(particle.k);
      std::vector<std::string> bloquesAdy = malla.blocks[block_key2].bloques_ady;

      for (std::string bloque : bloquesAdy) {
        std::vector<int> particulas_ady = malla.blocks[bloque].particles;
        for (int id: particulas_ady) {
          Particle & particula = particles[id];
          if (particula > particle) {
            double p_dif_x = particle.px- particula.px;
            double p_dif_y = particle.py- particula.py;
            double p_dif_z = particle.pz- particula.pz;
            double distancia = std::sqrt(p_dif_x* p_dif_x +p_dif_y * p_dif_y +p_dif_z* p_dif_z);
            double variacion_densidad = 0;
            if((distancia*distancia) < (suavizado*suavizado)) {
              variacion_densidad = (suavizado*suavizado - distancia*distancia)*(suavizado*suavizado - distancia*distancia)* (suavizado*suavizado - distancia*distancia);

              //std::cout<<"Variacion de densidad "<<variacion_densidad;
              particle.densidad = particle.densidad + variacion_densidad;
              //std::cout <<"Densidad actual "<< particle.densidad;
              //particles[particula.id].densidad = particles[particula.id].densidad + variacion_densidad;
              particula.densidad = particula.densidad + variacion_densidad;
            }
          }
        }
      }
      // Transformación de densidad
      //std::cout<< "Densidad nueva "<<particle.densidad;
      //std::cout<< "Suavizado "<<suavizado;
      particle.densidad = (particle.densidad + pow(suavizado, 6)) * 315 * masa/ (64 * M_PI * pow(suavizado, 9));
      //std::cout<< "Densidad nueva "<<particle.densidad;

      //Aceleraciones

    }
    for (int part = 0; part < numparticulas; part++) {
      Particle & particle = particles[part];
      std::string block_key2 = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" +
                               std::to_string(particle.k);
      std::vector<std::string> bloquesAdy = malla.blocks[block_key2].bloques_ady;
      for (std::string bloque : bloquesAdy) {
        std::vector<int> particulas_ady = malla.blocks[bloque].particles;
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
    for (std::string bloque: malla.colisionesCx){
      std::vector<int> particulas_0x = malla.blocks[bloque].particles;
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
    for (std::string bloque: malla.colisionesCy){
      std::vector<int> particulas_0y = malla.blocks[bloque].particles;
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
    for (std::string bloque: malla.colisionesCz){
      std::vector<int> particulas_0z = malla.blocks[bloque].particles;
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
    /*for (int part = 0; part < numparticulas; part++){
      Particle & particula = particles[part];
      double d_x = 1;
      if(particula.i == 0){
        d_x = particula.px - bmin_x;
      }
      else if(particula.i==nx-1){
        d_x = bmax_x - particula.px;
      }
      if(d_x<0){
        if (particula.i == 0){
          particula.px = bmin_x - d_x;
        }
        else if(particula.i==nx-1){
          particula.px = bmax_x + d_x;
        }
        particula.vx = -particula.vx;
        particula.hvx = -particula.hvx;
      }

      double d_y = 1;
      if(particula.j == 0){
        d_y = particula.py - bmin_y;
      }
      else if(particula.j==ny-1){
        d_y = bmax_y - particula.py;
      }
      if(d_y<0){
        if (particula.j == 0){
          particula.py = bmin_y - d_y;
        }
        else if(particula.j==nx-1){
          particula.py = bmax_y + d_y;
        }
        particula.vy = -particula.vy;
        particula.hvy = -particula.hvy;
      }

      double d_z = 1;
      if(particula.k == 0){
        d_z = particula.pz - bmin_z;
      }
      else if(particula.k==nz-1){
        d_z = bmax_z - particula.pz;
      }
      if(d_z<0){
        if (particula.k == 0){
          particula.pz = bmin_z - d_z;
        }
        else if(particula.k==nz-1){
          particula.pz = bmax_z + d_z;
        }
        particula.vz = -particula.vz;
        particula.hvz = -particula.hvz;
      }

      if(time==args.nts -1){
        outputfile << "ID: " << particula.id << std::endl;
        outputfile << "Posición (x, y, z): " << particula.px << ", " << particula.py << ", " << particula.pz << std::endl;
        outputfile << "Velocidad (vx, vy, vz): " << particula.vx << ", " << particula.vy << ", " << particula.vz << std::endl;
        outputfile << "Hvx, Hvy, Hvz: " << particula.hvx << ", " << particula.hvy << ", " << particula.hvz << std::endl;
        outputfile << "Densidad: " << particula.densidad << std::endl;
        outputfile << "Aceleración (accx, accy, accz): " << particula.ax << ", " << particula.ay << ", " << particula.az << std::endl;
      }

      particula.densidad = 0;
      particula.ax = g_x;
      particula.ay = g_y;
      particula.az = g_z;

      int i_anterior = particula.i;
      int j_anterior = particula.j;
      int k_anterior = particula.k;

      particula.i = static_cast<int>((particula.px - bmin_x) / sx);

      if (particula.i > nx - 1) {
        particula.i = nx - 1;
      }
      if(particula.i<0){
        particula.i=0;
      }
      particula.j = static_cast<int>((particula.py - bmin_y) / sy);
      if (particula.j > ny - 1) {
        particula.j = ny - 1;
      }
      if(particula.j<0){
        particula.j=0;
      }
      particula.k = static_cast<int>((particula.pz - bmin_z) / sz);
      if (particula.k > nz - 1) {
        particula.k = nz - 1;
      }
      if(particula.k<0){
        particula.k=0;
      }
      if (i_anterior != particula.i || j_anterior != particula.j || k_anterior != particula.k) {
        std::string block_key = std::to_string(i_anterior) + "_" + std::to_string(j_anterior) +
                                "_" + std::to_string(k_anterior);
        malla.blocks[block_key].removeParticle(particula.id);
        std::string block_key2 = std::to_string(particula.i) + "_" + std::to_string(particula.j) +
                                 "_" + std::to_string(particula.k);
        malla.blocks[block_key2].addParticle(particula.id);
      }
    }*/
    for (std::string bloque: malla.colisionesCx) {
      std::vector<int> particulas_0x = malla.blocks[bloque].particles;
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

    for (std::string bloque: malla.colisionesCy) {
      std::vector<int> particulas_0y = malla.blocks[bloque].particles;
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
          //std::cout<<"Entra "<<particula.id <<"Bloque: "<<particula.i<<particula.j<<particula.k<<"\n";

          if (malla.blocks[bloque].j == 0){
            particula.py = bmin_y - d_y;
            //std::cout<<"Entra "<<particula.id <<"Bloque: "<<particula.i<<particula.j<<particula.k<<"\n";
          }
          else{
            particula.py = bmax_y + d_y;
          }
          particula.vy = -particula.vy;
          particula.hvy = -particula.hvy;
        }
      }
    }
    for (std::string bloque: malla.colisionesCz) {
      std::vector<int> particulas_0z = malla.blocks[bloque].particles;
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
    /*Particle particle = particles[1120];
    std::cout<<"--------------------------------------------\n";
    std::cout<<"Después del bucle. Densidad" << particle.densidad;
    std::cout<<"Después del bucle. X: "<<particle.px<<"Y: "<<particle.py<<"z: "<<particle.pz<<"\n";
    std::cout<<"Después del bucle. VX: "<<particle.vx<<"vY: "<<particle.vy<<"vz: "<<particle.vz<<"\n";
    std::cout<<"Después del bucle. aX: "<<particle.ax<<"aY: "<<particle.ay<<"az: "<<particle.az<<"\n";
    std::cout<<"--------------------------------------------\n";*/
  }
  // Mostrar los datos de las partículas
  for (Particle & particle: particles) {
    outputfile << "ID: " << particle.id << std::endl;
    outputfile << "Posición (x, y, z): " << particle.px << ", " << particle.py << ", " << particle.pz << std::endl;
    outputfile << "Velocidad (vx, vy, vz): " << particle.vx << ", " << particle.vy << ", " << particle.vz << std::endl;
    outputfile << "Hvx, Hvy, Hvz: " << particle.hvx << ", " << particle.hvy << ", " << particle.hvz << std::endl;
    outputfile << "Densidad: " << particle.densidad << std::endl;
    outputfile << "Aceleración (accx, accy, accz): " << particle.ax << ", " << particle.ay << ", " << particle.az << std::endl;
  }
  inputfile.close();
  return 0;
}