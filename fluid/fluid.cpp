#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../sim/progargs.hpp"

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
    float px, py, pz;   // Coordenadas de posición
    float hvx, hvy, hvz; // Coordenadas del vector hv
    float vx, vy, vz;   // Coordenadas de la velocidad
};

int main(int argc, char *argv[]) {
    ProgArgs args(argc, argv);

    std::ifstream inputfile(args.inputfile, std::ios::binary);
    std::ofstream outputfile(args.outputfile, std::ios::binary);

    float ppm;
    int np;

    // Lectura de la cabecera
    inpufile.read(reinterpret_cast<char *>(&ppm), sizeof(float));
    inputfile.read(reinterpret_cast<char *>(&np), sizeof(int));

    std::cout << "Partículas por metro: " << ppm << std::endl;
    std::cout << "Número de partículas: " << np << std::endl;

    // Lectura de la información de cada partícula
    std::vector<Particle> particles(np);

    for (int i = 0; i < np; ++i) {
        inpufile.read(reinterpret_cast<char *>(&particles[i].px), sizeof(float));
        inpufile.read(reinterpret_cast<char *>(&particles[i].py), sizeof(float));
        inpufile.read(reinterpret_cast<char *>(&particles[i].pz), sizeof(float));
        inpufile.read(reinterpret_cast<char *>(&particles[i].hvx), sizeof(float));
        inpufile.read(reinterpret_cast<char *>(&particles[i].hvy), sizeof(float));
        inpufile.read(reinterpret_cast<char *>(&particles[i].hvz), sizeof(float));
        inpufile.read(reinterpret_cast<char *>(&particles[i].vx), sizeof(float));
        inpufile.read(reinterpret_cast<char *>(&particles[i].vy), sizeof(float));
        inpufile.read(reinterpret_cast<char *>(&particles[i].vz), sizeof(float));
    }

    // Mostrar los datos de las partículas
    /* for (int i = 0; i < np; ++i) {
        std::cout << "Partícula " << i + 1 << ":\n";
        std::cout << "Posición: (" << particles[i].px << ", " << particles[i].py << ", " << particles[i].pz << ")\n";
        std::cout << "Vector hv: (" << particles[i].hvx << ", " << particles[i].hvy << ", " << particles[i].hvz << ")\n";
        std::cout << "Velocidad: (" << particles[i].vx << ", " << particles[i].vy << ", " << particles[i].vz << ")\n\n";
    }
 */
    inpufile.close();

   /*  int x = read_binary_value<int>(inputfile);
    std::cout << "El valor leído es: " << x << '\n';
    write_binary_value(x, outputfile);

    float y = read_binary_value<float>(inputfile); */
    // Procede con la simulación aquí

    return 0;
}
