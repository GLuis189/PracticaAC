//
// Created by luis on 18/11/23.
//
#include "gtest/gtest.h"
#include "../sim/simulacion.cpp"
#include "../sim/progargs.hpp"

/*TEST(PruebaFuncional, CompararConTrazas1_large) {
  ProgArgs args;
  args.nts = 1;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile = "../large.fld";
  args.outputfile = "small1_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  std::vector<Particle> resultado = leerArchivo(args.outputfile);

  std::vector<Particle> trazas = leerArchivo("../trazas_large_1_final.out");

  ASSERT_EQ(resultado.size(), trazas.size()) << "El número de partículas no coincide con las trazas";

  for (size_t i = 0; i < resultado.size(); ++i) {
    EXPECT_EQ(resultado[i].id, trazas[i].id) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].px, trazas[i].px) << "Fallo en px de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].py, trazas[i].py) << "Fallo en py de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].pz, trazas[i].pz) << "Fallo en pz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].hvx, trazas[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].hvy, trazas[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].hvz, trazas[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].vx, trazas[i].vx) << "Fallo en vx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].vy, trazas[i].vy) << "Fallo en vy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].vz, trazas[i].vz) << "Fallo en vz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].densidad, trazas[i].densidad) << "Fallo en densidad de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].ax, trazas[i].ax) << "Fallo en ax de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].ay, trazas[i].ay) << "Fallo en ay de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].az, trazas[i].az) << "Fallo en az de la partícula " << i;
  }
}*/
TEST(PruebaFuncional, CompararConTrazas5_large) {
  std::vector<Particle> resultado = leerArchivo("../small1_binario.out", false);
  std::vector<Particle> trazas = leerArchivo("../trazas_small_1_binario.out", true);

  //ASSERT_EQ(resultado.size(), trazas.size()) << "El número de partículas no coincide con las trazas";

  for (size_t i = 0; i < resultado.size(); ++i) {
    EXPECT_EQ(resultado[i].id, trazas[i].id) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].px, trazas[i].px) << "Fallo en px de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].py, trazas[i].py) << "Fallo en py de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].pz, trazas[i].pz) << "Fallo en pz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].hvx, trazas[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].hvy, trazas[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].hvz, trazas[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].vx, trazas[i].vx) << "Fallo en vx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].vy, trazas[i].vy) << "Fallo en vy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado[i].vz, trazas[i].vz) << "Fallo en vz de la partícula " << i;
  }
}
