//
// Created by luis on 18/11/23.
//
#include "gtest/gtest.h"
#include "../sim/simulacion.cpp"
#include "../sim/progargs.hpp"

/*bool compararArchivos(const std:: string& archivo1, const std:: string& archivo2){
  std::ifstream a_1(archivo1, std::ios::binary);
  std::ifstream  a_2(archivo2, std::ios::binary);

 return std::equal(
      std
      )
}*/

/*TEST(FileComparison, BinaryFiles) {
    const char* filePath1 = "archivo1.bin";
    const char* filePath2 = "archivo2.bin";

  std::ifstream file1(filePath1, std::ios::binary);
  std::ifstream file2(filePath2, std::ios::binary);

  ASSERT_TRUE(file1.is_open()) << "No se pudo abrir el archivo " << filePath1;
  ASSERT_TRUE(file2.is_open()) << "No se pudo abrir el archivo " << filePath2;

  // Lee ppm y el número de partículas
  float ppm1, ppm2;
  int numParticles1, numParticles2;

  file1.read(reinterpret_cast<char*>(&ppm1), sizeof(float));
  file1.read(reinterpret_cast<char*>(&numParticles1), sizeof(int));

  file2.read(reinterpret_cast<char*>(&ppm2), sizeof(float));
  file2.read(reinterpret_cast<char*>(&numParticles2), sizeof(int));

  ASSERT_EQ(ppm1, ppm2) << "Los archivos tienen diferentes ppm";
  ASSERT_EQ(numParticles1, numParticles2) << "Los archivos tienen diferentes números de partículas";

  // Lee los datos de las partículas
  std::vector<Particle> particles1, particles2;
  Particle particle;

  while (file1.read(reinterpret_cast<char*>(&particle), sizeof(Particle))) {
    particles1.push_back(particle);
  }

  while (file2.read(reinterpret_cast<char*>(&particle), sizeof(Particle))) {
    particles2.push_back(particle);
  }

  ASSERT_EQ(particles1.size(), particles2.size()) << "Los archivos tienen tamaños de partículas diferentes";

  // Compara cada partícula
  for (size_t i = 0; i < particles1.size(); ++i) {
    EXPECT_TRUE(compareParticles(particles1[i], particles2[i]))
        << "Diferencia en la partícula " << i;
  }

  file1.close();
  file2.close();
}*/

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

    archivo.read(reinterpret_cast<char*>(&particle.px), sizeof(particle.px));
    archivo.read(reinterpret_cast<char*>(&particle.py), sizeof(particle.py));
    archivo.read(reinterpret_cast<char*>(&particle.pz), sizeof(particle.pz));

    archivo.read(reinterpret_cast<char*>(&particle.hvx), sizeof(particle.hvx));
    archivo.read(reinterpret_cast<char*>(&particle.hvy), sizeof(particle.hvy));
    archivo.read(reinterpret_cast<char*>(&particle.hvz), sizeof(particle.hvz));

    archivo.read(reinterpret_cast<char*>(&particle.vx), sizeof(particle.vx));
    archivo.read(reinterpret_cast<char*>(&particle.vy), sizeof(particle.vy));
    archivo.read(reinterpret_cast<char*>(&particle.vz), sizeof(particle.vz));

    archivoData.particles.push_back(particle);
  }

  archivo.close();
  return archivoData;
}
bool sonArchivosIguales(const std::string& archivo1, const std::string& archivo2) {
  std::ifstream file1(archivo1, std::ios::binary);
  std::ifstream file2(archivo2, std::ios::binary);

  char byte1, byte2;
  int contador = 1;
  while (file1.get(byte1) && file2.get(byte2)) {
    // Comparar byte a byte
    std::cout<<contador<<"\n";
    contador++;
    if (byte1 != byte2) {
      return false;
    }
  }

  // Si uno de los archivos llegó al final, el otro también debe haberlo hecho para ser iguales
  return file1.eof() && file2.eof();
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

  ASSERT_EQ(resultado.particles.size(), trazas.particles.size());
  ASSERT_EQ(resultado.ppm, trazas.ppm);
  ASSERT_EQ(resultado.numParticulas, trazas.numParticulas)
      << "El número de partículas no coincide con las trazas";
  for (size_t i = 0; i < resultado.particles.size(); ++i) {
    EXPECT_EQ(resultado.particles[i].id, trazas.particles[i].id) << "Fallo en la id de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].px, trazas.particles[i].px) << "Fallo en px de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].py, trazas.particles[i].py) << "Fallo en py de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].pz, trazas.particles[i].pz) << "Fallo en pz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvx, trazas.particles[i].hvx) << "Fallo en hvx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvy, trazas.particles[i].hvy) << "Fallo en hvy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].hvz, trazas.particles[i].hvz) << "Fallo en hvz de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].vx, trazas.particles[i].vx) << "Fallo en vx de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].vy, trazas.particles[i].vy) << "Fallo en vy de la partícula " << i;
    EXPECT_DOUBLE_EQ(resultado.particles[i].vz, trazas.particles[i].vz) << "Fallo en vz de la partícula " << i;
  }
}
/*TEST(ArchivoTest, ArchivosIguales) {
  ProgArgs args;
  args.nts = 1;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile = "../small.fld";
  args.outputfile = "small1_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);
  const std::string archivo1 = args.outputfile;
  const std::string archivo2 = "../out/small-1.fld";

  ASSERT_TRUE(sonArchivosIguales(archivo1, archivo2)) << "Los archivos no son iguales";
}

TEST(PruebaFuncional, CompararConTrazas1_large) {
  ProgArgs args;
  args.nts = 1;  // Reemplaza esto con el número de iteraciones que quieras
  args.inputfile = "../small.fld";
  args.outputfile = "small1_test.out";

  // Ejecuta la simulación
  std::ifstream inputfile(args.inputfile, std::ios::binary);
  std::ofstream outputfile(args.outputfile, std::ios::binary);
  grid malla = calcularMalla(inputfile);
  std::vector<Particle> particles = leerParticulas(inputfile, malla);
  IniciarSimulacion(args, outputfile, malla, particles);

  // Lee los resultados de la simulación
  std::vector<Particle> resultado = leerArchivo(args.outputfile);

  std::vector<Particle> trazas = leerArchivo("../out/small-1.fld");
  float ppm_resultado, ppm_trazas;
  int numParticulas_resultado, numParticulas_trazas;

  std::ifstream resultado_cabecera(args.outputfile, std::ios::binary);
  inputfile.read(reinterpret_cast<char *>(&ppm_resultado), sizeof(ppm_resultado));
  inputfile.read(reinterpret_cast<char *>(&numParticulas_resultado), sizeof(numParticulas_resultado));

  std::ifstream trazasfile("../out/small-1.fld", std::ios::binary);
  trazasfile.read(reinterpret_cast<char *>(&ppm_trazas), sizeof(ppm_trazas));
  trazasfile.read(reinterpret_cast<char *>(&numParticulas_trazas), sizeof(numParticulas_trazas));

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
  }
}

TEST(PruebaFuncional, CompararConTrazas5_large) {
  std::vector<Particle> resultado = leerArchivo("../small1_binario.out");
  std::vector<Particle> trazas = leerArchivo("../trazas_small_1_binario.out");

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
}*/
