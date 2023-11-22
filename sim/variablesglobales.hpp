#include <fstream>

#ifndef VARIABLESGLOBALES_H
#define VARIABLESGLOBALES_H

//NOLINTBEGIN
template <typename T>
typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, char*>
    as_writable_buffer(T& value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<char*>(&value);
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
//NOLINTEND

const double radio           = 1.695;
const double densidad        = 1e3;
const double presion         = 3.0;
const double colisiones      = 3e4;
const double amortiguamiento = 128.0;
const double vis             = 0.4;  // Asume un valor para μ_p
const double tparticula      = 2e-4;
const double ptiempo         = 1e-3;  // Asume un valor para Δt
const double var_const       = 1e-10;
const int errorconst         = -5;
const int n_5 = 5;
const int n_6 = 6;
const int n_9 = 9;
const int n_64 = 64;
const int n_315 = 315;
const int dmil = 10000;
const int cien = 100;
const double g_y = -9.8;

const double bmax_x = 0.065;
const double bmax_y = 0.1;
const double bmax_z = 0.065;
const double bmin_x = -0.065;
const double bmin_y = -0.08;
const double bmin_z = -0.065;

const double suavizadot = 0.00580479;

const double n_05 = 0.5;

#endif // VARIABLESGLOBALES_H
