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

// Estructura para representar cada partícula

void calcularBloquesAdyacentes(Particle& p, std::vector<Block> bloques_ady, grid malla){
  for (int i = -1; i < 2; i++){
    for (int j = -1; i < 2; i++){
      for (int k = -1; i < 2; i++){
        std::string block_key = std::to_string(p.i + i) + "_" + std::to_string(p.j + j) + "_" + std::to_string(p.k + k);
        bloques_ady.push_back(malla.blocks[block_key]);
      }
    }
  }
}

int main(int argc, char *argv[]) {
    ProgArgs args(argc, argv);

    std::ifstream inputfile(args.inputfile, std::ios::binary);
    std::ofstream outputfile(args.outputfile, std::ios::binary);

    const float ppm         = read_binary_value<float>(inputfile);
    const int numparticulas = read_binary_value<int>(inputfile);
    const float radio           = 1.695;
    const float densidad        = 1e3;
    const float presion         = 3.0;
    const float colisiones      = 3e4;
    const float amortiguamiento = 128.0;
    const float vis             = 0.4;  // Asume un valor para μ_p
    const float tparticula      = 2e-4;
    const float ptiempo         = 1e-3;  // Asume un valor para Δt

    const float g_x = 0.0;
    const float g_y = -9.8;
    const float g_z = 0.0;

    const float bmax_x = 0.065;
    const float bmax_y = 0.1;
    const float bmax_z = 0.065;

    const float bmin_x = -0.065;
    const float bmin_y = -0.08;
    const float bmin_z = -0.065;

    const float masa = densidad / pow(ppm, 3);

    if (numparticulas <= 0) {
      std::cerr << "Error: Invalid number of particles: 0.\n";
      exit(-5);
    }

    const float suavizado = radio / ppm;
    int nx                = static_cast<int>((bmax_x - bmin_x) / suavizado);
    int ny                = static_cast<int>((bmax_y - bmin_y) / suavizado);
    int nz                = static_cast<int>((bmax_z - bmin_z) / suavizado);
    grid malla(nx, ny, nz);

    float sx = (bmax_x - bmin_x) / nx;
    float sy = (bmax_y - bmin_y) / ny;
    float sz = (bmax_z - bmin_z) / nz;

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

      Particle::calcularBloque(particle, bmin_x, sx, bmin_y, sy, bmin_z, sz, nx, ny, nz);

      std::string block_key = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" +
                              std::to_string(particle.k);
      malla.blocks[block_key].addParticle(particle);
      particles.push_back(particle);
      ++contar_particulas;
    }

    // Crear archivo con los bloques
    /*if (outputfile.is_open()) {
        for (const auto& block : malla.blocks) {
          outputfile << "Block Key: " << block.first << "\n";
          // Aquí puedes imprimir detalles específicos de los bloques si es necesario
        }
        outputfile.close();
    } else {
        std::cerr << "Unable to open file: " << "\n";
    }*/
    if (numparticulas != contar_particulas) {
      std::cerr << "Error: Number of particles mismatch. Header:" << numparticulas << " Found:" << contar_particulas << "\n";
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

    // Mostrar los datos de las partículas
    /*for (int i = 0; i < numparticulas; ++i) {
        outputfile << "Partícula " << i + 1 << ":\n";
        outputfile << "Posición: (" << particles[i].px << ", " << particles[i].py << ", " <<
    particles[i].pz << ")\n"; outputfile << "Vector hv: (" << particles[i].hvx << ", " <<
    particles[i].hvy << ", " << particles[i].hvz << ")\n"; outputfile << "Velocidad: (" <<
    particles[i].vx << ", " << particles[i].vy << ", " << particles[i].vz << ")\n\n";
    }

    inputfile.close();*/

    /*  int x = read_binary_value<int>(inputfile);
     std::cout << "El valor leído es: " << x << '\n';
     write_binary_value(x, outputfile);

     float y = read_binary_value<float>(inputfile); */
    // Procede con la simulación aquí

    // Inicio de simulación
    for (int time = 0; time < args.nts; time++) {
      for (int part = 0; part < numparticulas; part++) {
        Particle & particle = particles[part];

        int i_anterior = particle.i;
        int j_anterior = particle.j;
        int k_anterior = particle.k;

        particle.i = static_cast<int>((particle.px - bmin_x) / sx);
        if (particle.i > nx - 1) {
            particle.i = nx - 1;
        }
        particle.j = static_cast<int>((particle.py - bmin_y) / sy);
        if (particle.j > ny - 1) {
            particle.j = ny - 1;
        }
        particle.k = static_cast<int>((particle.pz - bmin_z) / sz);
        if (particle.k > nz - 1) {
            particle.k = nz - 1;
        }

        if (i_anterior != particle.i || j_anterior != particle.j || k_anterior != particle.k) {
          std::string block_key = std::to_string(i_anterior) + "_" + std::to_string(j_anterior) + "_" + std::to_string(k_anterior);
          malla.blocks[block_key].removeParticle(particle);
          std::string block_key2 = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" + std::to_string(particle.k);
          malla.blocks[block_key2].addParticle(particle);
        }
        std::string block_key2 = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" + std::to_string(particle.k);
        std::vector<std::string> bloquesAdy = malla.blocks[block_key2].bloques_ady;
        for(std::string bloque : bloquesAdy){
          std::vector<Particle> particulas_ady = malla.blocks[bloque].particles;
          for (Particle & particula : particulas_ady){
            if (particula > particle) {
              double p_dif              = particle.densidad - particula.densidad;
              double variacion_densidad = 0;
              if (pow(p_dif, 2) < pow(suavizado, 2)) {
                variacion_densidad = (pow(suavizado, 2) - pow(p_dif, 2));
              }
              particle.densidad = particle.densidad + pow(variacion_densidad, 3);
              particles[particula.id].densidad = particles[particula.id].densidad + pow(variacion_densidad, 3);
            }
          }
        }

        /*for (int pos = particle.id +1; pos<numparticulas; pos++) {
          if (particle.i == particles[pos].i - 1 || particle.i == particles[pos].i || particle.i == particles[pos].i+1) {
            if (particle.j == particles[pos].j - 1 || particle.j == particles[pos].j || particle.j == particles[pos].j+1) {
              if (particle.k == particles[pos].j - 1 || particle.k == particles[pos].k || particle.k == particles[pos].k+1) {
                double p_dif = particle.densidad - particles[pos].densidad;
                double variacion_densidad = 0;
                if (pow(p_dif,2) < pow(suavizado,2)){
                  variacion_densidad = (pow(suavizado,2)-pow(p_dif,2));
                }
                particle.densidad = particle.densidad + pow(variacion_densidad,3);
                particles[pos].densidad = particles[pos].densidad + pow(variacion_densidad,3);
              }
            }
          }
        }*/
        /*for (Particle & particula : particulas_ady) {
          auto find = std::find(particulas_ady.begin(), particles.end(), particula);
            calcularDensidad(particle, particula, suavizado);
          }*/

      }
    }
    return 0;
  }