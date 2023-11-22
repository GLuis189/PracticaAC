//
// Created by luis on 18/11/23.
//
// NOLINTBEGIN
#include "gtest/gtest.h"
#include "../sim/simulacion.hpp"
#include "../sim/variablesglobales.hpp"
#include <fstream>

struct ArchivoData {
    float ppm{};
    int numParticulas{};
    std::vector<Particle> particles;
};

template <typename T>
void readValue(std::ifstream& file, T& value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  file.read(reinterpret_cast<char*>(&value), sizeof(T));
}

ArchivoData leerArchivo(const std::string& nombreArchivo) {
  std::ifstream archivo(nombreArchivo, std::ios::binary);
  ArchivoData archivoData;
  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo." << "\n";
    return archivoData;
  }
  readValue(archivo, archivoData.ppm);
  readValue(archivo, archivoData.numParticulas);
  while (archivo) {
    Particle particle;
    readValue(archivo, particle.posicion.c_x);
    readValue(archivo, particle.posicion.c_y);
    readValue(archivo, particle.posicion.c_z);
    readValue(archivo, particle.hvelocidad.c_x);
    readValue(archivo, particle.hvelocidad.c_y);
    readValue(archivo, particle.hvelocidad.c_z);
    readValue(archivo, particle.velocidad.c_x);
    readValue(archivo, particle.velocidad.c_y);
    readValue(archivo, particle.velocidad.c_z);
    archivoData.particles.emplace_back(particle);
  }
  archivo.close();
  return archivoData;
}

TEST(PruebaFuncional, CompararConTrazas1_small) {
  ProgArgs args;
  args.nts        = 1;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/small.fld";
  args.outputfile = "small1_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/small-1.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas2_small) {
  ProgArgs args;
  args.nts        = 2;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/small.fld";
  args.outputfile = "small2_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/small-2.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas3_small) {
  ProgArgs args;
  args.nts        = 3;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/small.fld";
  args.outputfile = "small3_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/small-3.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas4_small) {
  ProgArgs args;
  args.nts        = 4;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/small.fld";
  args.outputfile = "small4_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/small-4.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas5_small) {
  ProgArgs args;
  args.nts        = n_5;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/small.fld";
  args.outputfile = "small5_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/small-5.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas1_large) {
  ProgArgs args;
  args.nts        = 1;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/large.fld";
  args.outputfile = "large1_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/large-1.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas2_large) {
  ProgArgs args;
  args.nts        = 2;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/large.fld";
  args.outputfile = "large2_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/large-2.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas3_large) {
  ProgArgs args;
  args.nts        = 3;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/large.fld";
  args.outputfile = "large3_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/large-3.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas4_large) {
  ProgArgs args;
  args.nts        = 4;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/large.fld";
  args.outputfile = "large4_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/large-4.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas5_large) {
  ProgArgs args;
  args.nts        = n_5;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "in/large.fld";
  args.outputfile = "large5_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("out/large-5.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la ide de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_x, trazas.particles[i].posicion.c_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_y, trazas.particles[i].posicion.c_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].posicion.c_z, trazas.particles[i].posicion.c_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_x, trazas.particles[i].hvelocidad.c_x) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_y, trazas.particles[i].hvelocidad.c_y) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvelocidad.c_z, trazas.particles[i].hvelocidad.c_z) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_x, trazas.particles[i].velocidad.c_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_y, trazas.particles[i].velocidad.c_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].velocidad.c_z, trazas.particles[i].velocidad.c_z) << "Fallo en v_z de la partícula " << i;
  }
}
// NOLINTEND