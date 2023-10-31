#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../sim/progargs.hpp"
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
struct Particle {
    double px, py, pz;   // Coordenadas de posición
    double hvx, hvy, hvz; // Coordenadas del vector hv
    double vx, vy, vz;   // Coordenadas de la velocidad
};

struct Malla{
  int x, y, z;

};


int main(int argc, char *argv[]) {
    ProgArgs args(argc, argv);

    std::ifstream inputfile(args.inputfile, std::ios::binary);
    std::ofstream outputfile(args.outputfile, std::ios::binary);

    float ppm;
    int np;

    /*const float r = 1.695;
    const float d = 1e3;
    const float ps = 3.0;
    const float sc = 3e4;
    const float dv = 128.0;
    const float mu = 0.4; // Asume un valor para μ_p
    const float dp = 2e-4;
    const float pt = 1e-3; // Asume un valor para Δt

    const float gx = 0.0;
    const float gy = -9.8;
    const float gz = 0.0;

    const float bmax_x = 0.65;
    const float bmax_y = 0.1;
    const float bmax_z = 0.065; 
    
    const float bmin_x = -0.065;
    const float bmin_y = -0.08;
    const float bmin_z = -0.065;

    float masa = d/(ppm*ppm*ppm);
    float h = r/ppm;
    
    
    Malla n;



    // Lectura de la cabecera
    inputfile.read(reinterpret_cast<char *>(&ppm), sizeof(float));
    inputfile.read(reinterpret_cast<char *>(&np), sizeof(int));

    std::cout << "Partículas por metro: " << ppm << std::endl;
    std::cout << "Número de partículas: " << np << std::endl;

    // Lectura de la información de cada partícula
        std::vector<Particle> particles;
    particles.reserve(np);

    for (int i = 0; i < np; ++i) {
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

        particles.push_back(particle);
    }

    // Mostrar los datos de las partículas
    for (int i = 0; i < np; ++i) {
        outputfile << "Partícula " << i + 1 << ":\n";
        outputfile << "Posición: (" << particles[i].px << ", " << particles[i].py << ", " << particles[i].pz << ")\n";
        outputfile << "Vector hv: (" << particles[i].hvx << ", " << particles[i].hvy << ", " << particles[i].hvz << ")\n";
        outputfile << "Velocidad: (" << particles[i].vx << ", " << particles[i].vy << ", " << particles[i].vz << ")\n\n";
    }
 
    inputfile.close();

   /*  int x = read_binary_value<int>(inputfile);
    std::cout << "El valor leído es: " << x << '\n';
    write_binary_value(x, outputfile);

    float y = read_binary_value<float>(inputfile); */
    // Procede con la simulación aquí

    return 0;
}
