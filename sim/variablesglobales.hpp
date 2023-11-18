#include <fstream>

#ifndef VARIABLESGLOBALES_H
#define VARIABLESGLOBALES_H


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

#endif // VARIABLESGLOBALES_H