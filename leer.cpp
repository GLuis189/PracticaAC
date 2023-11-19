#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Particle {
    float px, py, pz;
    float hvx, hvy, hvz;
    float vx, vy, vz;
};

template <typename T>
typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, char*>
    as_writable_buffer(T& value) {
  return reinterpret_cast<char*>(&value);
}

template <typename T>
typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, const char*>
    as_buffer(const T& value) {
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
  if (argc < 2) {
    std::cerr << "Por favor, proporciona el nombre del archivo de entrada y de salida como argumentos." << std::endl;
    return 1;
  }

  std::string nombreArchivo = "./small1-binario.out";
  std::ifstream archivo(nombreArchivo, std::ios::binary);

  if (!archivo) {
    std::cerr << "No se pudo abrir el archivo de entrada: " << nombreArchivo << std::endl;
    return 1;
  }

  std::string nombreArchivoSalida = argv[1];
  std::ofstream archivoSalida(nombreArchivoSalida);

  if (!archivoSalida) {
    std::cerr << "No se pudo abrir el archivo de salida: " << nombreArchivoSalida << std::endl;
    return 1;
  }

  float particulasPorMetro = read_binary_value<float>(archivo);
  int totalParticulas = read_binary_value<int>(archivo);

  archivoSalida << "Particulas por metro: " << particulasPorMetro << std::endl;
  archivoSalida << "Total de particulas: " << totalParticulas << std::endl;

  while (!archivo.eof()) {
    Particle p;
    if (read_binary_values(archivo, p.px, p.py, p.pz, p.hvx, p.hvy, p.hvz, p.vx, p.vy, p.vz)) {
      archivoSalida << "Pos: (" << p.px << ", " << p.py << ", " << p.pz << ")\n";
      archivoSalida << "HV: (" << p.hvx << ", " << p.hvy << ", " << p.hvz << ")\n";
      archivoSalida << "Vel: (" << p.vx << ", " << p.vy << ", " << p.vz << ")\n";
    }
  }

  archivo.close();
  archivoSalida.close();
  return 0;
}
