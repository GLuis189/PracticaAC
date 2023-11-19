#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>



/*std::vector<Particle> leerParticulas(std::ifstream& inputfile) {
  int ppm           = read_binary_value<float>(inputfile);
  int numparticulas = read_binary_value<int>(inputfile);
  std::vector<Particle> particles;
  particles.reserve(numparticulas);
  int contar_particulas = 0;
  float px, py, pz, hvx, hvy, hvz, vx, vy, vz;
  while(read_binary_values(inputfile,  px, py, pz, hvx, hvy, hvz, vx, vy, vz)) {
    Particle particle;
    particle.px = px, particle.py = py, particle.pz = pz;
    particle.hvx = hvx, particle.hvy = hvy, particle.hvz = hvz;
    particle.vx = vx, particle.vy = vy, particle.vz = vz;
    particles.push_back(particle);
    ++contar_particulas;
  }
  inputfile.close();
  if (numparticulas != contar_particulas) {
    std::cerr << "Error: Number of particles mismatch. Header:" << numparticulas
              << ", Found:" << contar_particulas << ".\n";
    exit(-5);
  }
  return particles;
}*/


// Comparador personalizado para ordenar partículas por ID
/*bool compararPorId(const Particle& a, const Particle& b) {
  return a.id < b.id;
}*/
struct Particle {
    //int64_t id;
    float px, py, pz;
    float hvx, hvy, hvz;
    float vx, vy, vz;
    //double density;
    //double accx, accy, accz;
};

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
  std::string nombreArchivo = "./out/small-1.fld";
  std::ifstream archivo(nombreArchivo, std::ios::binary);

  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return 1;
  }

  int numParticulas;
  int ppm;
  archivo.read(reinterpret_cast<char *>(&ppm), sizeof(int));
  archivo.read(reinterpret_cast<char *>(&numParticulas), sizeof(int));

  std::vector<Particle> particles;

  for (int i = 0; i < numParticulas; ++i) {
    Particle particle;
    archivo.read(reinterpret_cast<char *>(&particle.px), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&particle.py), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&particle.pz), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&particle.hvx), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&particle.hvy), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&particle.hvz), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&particle.vx), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&particle.vy), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&particle.vz), sizeof(float));
    particles.push_back(particle);
  }

  archivo.close();
  std::cout<<"ppm: "<<ppm << "Numero de particulas: "<<numParticulas<< "\n";
  for (const Particle& particle : particles) {
    std::cout << "Posición (x, y, z): " << particle.px << ", " << particle.py << ", " << particle.pz << std::endl;
    std::cout << "Velocidad (vx, vy, vz): " << particle.vx << ", " << particle.vy << ", " << particle.vz << std::endl;
    std::cout << "Hvx, Hvy, Hvz: " << particle.hvx << ", " << particle.hvy << ", " << particle.hvz << std::endl;
  }

}
  /*std::ifstream inputfile(argv[1], std::ios::binary);
  std::ofstream outputfile(argv[2]);
  std::cout<<"hola";
  if (!inputfile.is_open()) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return 1;
  }

  int ppm           = read_binary_value<float>(inputfile);
  int numparticulas = read_binary_value<int>(inputfile);

  std::vector<Particle> particles;
  particles.reserve(numparticulas);
  for (int64_t i = 0; i < numparticulas; ++i) {
    Particle particle;
    float px          = read_binary_value<float>(inputfile);
    float py          = read_binary_value<float>(inputfile);
    float pz          = read_binary_value<float>(inputfile);
    float hvx          = read_binary_value<float>(inputfile);
    float hvy         = read_binary_value<float>(inputfile);
    float hvz          = read_binary_value<float>(inputfile);
    float vx          = read_binary_value<float>(inputfile);
    float vy          = read_binary_value<float>(inputfile);
    float vz          = read_binary_value<float>(inputfile);
    particles.push_back(particle);
  }

  inputfile.close();
  outputfile<< numparticulas<< " ";
  outputfile<<ppm<<" ";
  for (const Particle& particle : particles) {
    outputfile << particle.px << " " << particle.py << " " << particle.pz << " "
               << particle.hvx << " " << particle.hvy << " " << particle.hvz << " "
               << particle.vx << " " << particle.vy << " " << particle.vz << "\n";
  }
  outputfile.close();
  return 0;
}*/
  /*std::string nombreArchivo = "boundint-base-1.trz";
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
  std::ofstream archivoSalida("trazas_small_1_binario.out", std::ios::binary);
  if (!archivoSalida.is_open()) {
    std::cerr << "Error al abrir el archivo de salida." << std::endl;
    return 1;
  }

  // Escribir el número de bloques y partículas en el archivo de salida

  // Escribir cada partícula en el archivo de salida
  for (Particle& particle : particles) {
    archivoSalida.write(reinterpret_cast<const char*>(&particle.posx), sizeof(particle.posx));
    archivoSalida.write(reinterpret_cast<const char*>(&particle.posy), sizeof(particle.posy));
    archivoSalida.write(reinterpret_cast<const char*>(&particle.posz), sizeof(particle.posz));
    archivoSalida.write(reinterpret_cast<const char*>(&particle.hvx), sizeof(particle.hvx));
    archivoSalida.write(reinterpret_cast<const char*>(&particle.hvy), sizeof(particle.hvy));
    archivoSalida.write(reinterpret_cast<const char*>(&particle.hvz), sizeof(particle.hvz));
    archivoSalida.write(reinterpret_cast<const char*>(&particle.velx), sizeof(particle.velx));
    archivoSalida.write(reinterpret_cast<const char*>(&particle.vely), sizeof(particle.vely));
    archivoSalida.write(reinterpret_cast<const char*>(&particle.vlez), sizeof(particle.vlez));
  }

  archivoSalida.close();
  // Imprimir el vector ordenado*/

  /*for (const Particle& particle : particles) {
    std::cout << "ID: " << particle.id << std::endl;
    std::cout << "Posición (x, y, z): " << particle.posx << ", " << particle.posy << ", " << particle.posz << std::endl;
    std::cout << "Velocidad (vx, vy, vz): " << particle.velx << ", " << particle.vely << ", " << particle.vlez << std::endl;
    std::cout << "Hvx, Hvy, Hvz: " << particle.hvx << ", " << particle.hvy << ", " << particle.hvz << std::endl;
    std::cout << "Densidad: " << particle.density << std::endl;
    std::cout << "Aceleración (accx, accy, accz): " << particle.accx << ", " << particle.accy << ", " << particle.accz << std::endl;
  }*/

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