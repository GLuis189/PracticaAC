//
// Created by luis on 18/11/23.
//
#include "gtest/gtest.h"
#include "../sim/simulacion.hpp"
#include "../sim/particle.hpp"
#include "../sim/grid.hpp"
#include "../sim/progargs.hpp"
#include <fstream>

struct ArchivoData {
    float ppm;
    int numParticulas;
    std::vector<Particle> particles;
};

ArchivoData leerArchivo(const std::string& nombreArchivo) {
  std::ifstream archivo(nombreArchivo, std::ios::binary);
  ArchivoData archivoData;

  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return archivoData;
  }

  archivo.read(reinterpret_cast<char*>(&archivoData.ppm), sizeof(archivoData.ppm));
  archivo.read(reinterpret_cast<char*>(&archivoData.numParticulas), sizeof(archivoData.numParticulas));

  while (!archivo.eof()) {
    Particle particle;

    archivo.read(reinterpret_cast<char*>(&particle.p_x), sizeof(particle.p_x));
    archivo.read(reinterpret_cast<char*>(&particle.p_y), sizeof(particle.p_y));
    archivo.read(reinterpret_cast<char*>(&particle.p_z), sizeof(particle.p_z));

    archivo.read(reinterpret_cast<char*>(&particle.hvx), sizeof(particle.hvx));
    archivo.read(reinterpret_cast<char*>(&particle.hvy), sizeof(particle.hvy));
    archivo.read(reinterpret_cast<char*>(&particle.hvz), sizeof(particle.hvz));

    archivo.read(reinterpret_cast<char*>(&particle.v_x), sizeof(particle.v_x));
    archivo.read(reinterpret_cast<char*>(&particle.v_y), sizeof(particle.v_y));
    archivo.read(reinterpret_cast<char*>(&particle.v_z), sizeof(particle.v_z));

    archivoData.particles.emplace_back(particle);
  }

  archivo.close();
  return archivoData;
}

TEST(PruebaFuncional, CompararConTrazas1_small) {
  ProgArgs args;
  args.nts        = 1;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../small.fld";
  args.outputfile = "small1_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/small-1.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas2_small) {
  ProgArgs args;
  args.nts        = 2;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../small.fld";
  args.outputfile = "small2_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/small-2.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas3_small) {
  ProgArgs args;
  args.nts        = 3;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../small.fld";
  args.outputfile = "small3_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/small-3.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas4_small) {
  ProgArgs args;
  args.nts        = 4;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../small.fld";
  args.outputfile = "small4_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/small-4.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas5_small) {
  ProgArgs args;
  args.nts        = 5;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../small.fld";
  args.outputfile = "small5_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/small-5.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas1_large) {
  ProgArgs args;
  args.nts        = 1;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../large.fld";
  args.outputfile = "large1_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/large-1.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas2_large) {
  ProgArgs args;
  args.nts        = 2;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../large.fld";
  args.outputfile = "large2_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/large-2.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas3_large) {
  ProgArgs args;
  args.nts        = 3;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../large.fld";
  args.outputfile = "large3_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/large-3.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas4_large) {
  ProgArgs args;
  args.nts        = 4;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../large.fld";
  args.outputfile = "large4_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/large-4.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}

TEST(PruebaFuncional, CompararConTrazas5_large) {
  ProgArgs args;
  args.nts        = 5;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile  = "../large.fld";
  args.outputfile = "large5_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla                      = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  ArchivoData resultado = leerArchivo(args.outputfile);
  ArchivoData trazas    = leerArchivo("../out/large-5.fld");

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size()) << "El número de partículas no coincide con las trazas";;
  ASSERT_EQ(resultado.ppm, trazas.ppm) << "El número de partículas por metro no coincide con las trazas";;
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas) << "El tamaño del archivo no coincide con las trazas";;

  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].ide, trazas.particles[i].ide) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_x, trazas.particles[i].p_x) << "Fallo en p_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_y, trazas.particles[i].p_y) << "Fallo en p_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].p_z, trazas.particles[i].p_z) << "Fallo en p_z de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_x, trazas.particles[i].v_x) << "Fallo en v_x de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_y, trazas.particles[i].v_y) << "Fallo en v_y de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].v_z, trazas.particles[i].v_z) << "Fallo en v_z de la partícula " << i;
  }
}
