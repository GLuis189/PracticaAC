#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../sim/progargs.hpp"

template <typename T>
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
}

int main(int argc, char *argv[]) {
    ProgArgs args(argc, argv);

    std::ifstream inputfile(args.inputfile, std::ios::binary);
    std::ofstream outputfile(args.outputfile, std::ios::binary);

    int x = read_binary_value<int>(inputfile);
    std::cout << "El valor leído es: " << x << '\n';
    write_binary_value(x, outputfile);

    float y = read_binary_value<float>(inputfile);
    // Procede con la simulación aquí

    return 0;
}
