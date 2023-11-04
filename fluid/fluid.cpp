#include <math.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../sim/progargs.hpp"
#include "../sim/grid.hpp"
#include "../sim/block.hpp"
#include "../sim/particle.hpp"
#include <vector>

/* template <typename T>
char * as_writable_buffer(T & value) {
  return reinterpret_cast<char *>(&value);
}

template <typename T>
char const * as_buffer(T const & value) {
  return reinterpret_cast<char const *>(&value);
}

template <typename T>
T read_binary_value(std::istream & is) {
  T value{};
  is.read(as_writable_buffer(value), sizeof(value));
  return value;
}

template <typename T>
void write_binary_value(T value, std::ostream & os) {
  os.write(as_buffer(value), sizeof(value));
} */

// Estructura para representar cada partícula


struct Malla{
  int x, y, z;

};


int main(int argc, char *argv[]) {
    ProgArgs args(argc, argv);

    std::ifstream inputfile(args.inputfile, std::ios::binary);
    std::ofstream outputfile(args.outputfile, std::ios::binary);

    float ppm = 0.0;
    int numparticulas = 0;

    const float radio = 1.695;
    const float densidad = 1e3;
    const float presion = 3.0;
    const float colisiones = 3e4;
    const float amortiguamiento = 128.0;
    const float vis = 0.4; // Asume un valor para μ_p
    const float tparticula = 2e-4;
    const float ptiempo = 1e-3; // Asume un valor para Δt

    const float g_x = 0.0;
    const float g_y = -9.8;
    const float g_z = 0.0;

    const float bmax_x = 0.65;
    const float bmax_y = 0.1;
    const float bmax_z = 0.065; 

    const float bmin_x = -0.065;
    const float bmin_y = -0.08;
    const float bmin_z = -0.065;

    const float masa = densidad/(ppm*ppm*ppm);
    
    // Lectura de la cabecera
    inputfile.read(reinterpret_cast<char *>(&ppm), sizeof(float));
    inputfile.read(reinterpret_cast<char *>(&numparticulas), sizeof(int));
    if (numparticulas<=0){
      std::cerr << "Error: Invalid number of particles: 0.\n";
      exit(-5);
    }

    std::cout << "Partículas por metro: " << ppm << std::endl;
    std::cout << "Número de partículas: " << numparticulas << std::endl;

    // Lectura de la información de cada partícula
    std::vector<Particle> particles;
    particles.reserve(numparticulas);
    int contar_particulas = 0;

    while(!inputfile.eof()) {
        Particle particle;

        float tempValue;
        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.px = tempValue;

        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.py = tempValue;

        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.pz = tempValue;

        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.hvx = tempValue;

        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.hvy = tempValue;

        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.hvz = tempValue;

        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.vx = tempValue;

        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.vy = tempValue;

        inputfile.read(reinterpret_cast<char *>(&tempValue), sizeof(float));
        particle.vz = tempValue;

        particle.i = static_cast<int>((particle.px-bmin_x)/sx);
        if (particle.i>nx -1){
          particle.i = nx -1;
        }
        particle.j = static_cast<int>((particle.py-bmin_y)/sy);
        if (particle.j>ny -1){
          particle.j = ny-1;
        }
        particle.k = static_cast<int>((particle.pz-bmin_z)/sz);
        if (particle.k>nz -1){
          particle.k = nz -1;
        }

        std::string block_key = std::to_string(particle.i) + "_" + std::to_string(particle.j) + "_" + std::to_string(particle.k);
        malla.blocks[block_key].addParticle(particle);
        particles.push_back(particle);
        ++contar_particulas;
    }
    if (outputfile.is_open()) {
        for (const auto& block : malla.blocks) {
          outputfile << "Block Key: " << block.first << std::endl;
          // Aquí puedes imprimir detalles específicos de los bloques si es necesario
        }
        outputfile.close();
    } else {
        std::cerr << "Unable to open file: " << std::endl;
    }
    if (numparticulas!=contar_particulas){
        std::cerr << "Error: Number of particles mismatch. Header:"<< numparticulas <<"Found:" << contar_particulas<<"\n";
    }
    // Mostrar los datos de las partículas
    /*for (int i = 0; i < numparticulas; ++i) {
        outputfile << "Partícula " << i + 1 << ":\n";
        outputfile << "Posición: (" << particles[i].px << ", " << particles[i].py << ", " << particles[i].pz << ")\n";
        outputfile << "Vector hv: (" << particles[i].hvx << ", " << particles[i].hvy << ", " << particles[i].hvz << ")\n";
        outputfile << "Velocidad: (" << particles[i].vx << ", " << particles[i].vy << ", " << particles[i].vz << ")\n\n";
    }
 
    inputfile.close();*/

   /*  int x = read_binary_value<int>(inputfile);
    std::cout << "El valor leído es: " << x << '\n';
    write_binary_value(x, outputfile);

    float y = read_binary_value<float>(inputfile); */
    // Procede con la simulación aquí

    return 0;
}
