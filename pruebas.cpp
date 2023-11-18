#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Particle {
    int64_t id;
    double posx, posy, posz;
    double hvx, hvy, hvz;
    double velx, vely, vlez;
    double density;
    double accx, accy, accz;
};

// Comparador personalizado para ordenar partículas por ID
bool compararPorId(const Particle& a, const Particle& b) {
  return a.id < b.id;
}

int main() {
  std::string nombreArchivo = "large-1.fld";
  std::ifstream archivo(nombreArchivo, std::ios::binary);

  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return 1;
  }

  int numBloques;
  archivo.read(reinterpret_cast<char*>(&numBloques), sizeof(int));

  std::vector<Particle> particles;

  for (int block = 0; block < numBloques; ++block) {
    int64_t numParticulas;
    archivo.read(reinterpret_cast<char*>(&numParticulas), sizeof(int64_t));

    for (int64_t i = 0; i < numParticulas; ++i) {
      Particle particle;
      archivo.read(reinterpret_cast<char*>(&particle.id), sizeof(int64_t));
      archivo.read(reinterpret_cast<char*>(&particle.posx), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.posy), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.posz), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.hvx), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.hvy), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.hvz), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.velx), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.vely), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.vlez), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.density), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.accx), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.accy), sizeof(double));
      archivo.read(reinterpret_cast<char*>(&particle.accz), sizeof(double));

      particles.push_back(particle);
    }
  }

  archivo.close();

  // Ordenar el vector de partículas por ID
  std::sort(particles.begin(), particles.end(), compararPorId);

  // Imprimir el vector ordenado
  for (const Particle& particle : particles) {
    std::cout << "ID: " << particle.id << std::endl;
    std::cout << "Posición (x, y, z): " << particle.posx << ", " << particle.posy << ", " << particle.posz << std::endl;
    std::cout << "Velocidad (vx, vy, vz): " << particle.velx << ", " << particle.vely << ", " << particle.vlez << std::endl;
    std::cout << "Hvx, Hvy, Hvz: " << particle.hvx << ", " << particle.hvy << ", " << particle.hvz << std::endl;
    std::cout << "Densidad: " << particle.density << std::endl;
    std::cout << "Aceleración (accx, accy, accz): " << particle.accx << ", " << particle.accy << ", " << particle.accz << std::endl;
  }

  return 0;
}
/*while (!inputfile.eof()) {
  Particle  particle;
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
}*/