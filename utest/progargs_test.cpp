#include "gtest/gtest.h"
#include "../sim/progargs.hpp"
#include "../sim/variablesglobales.hpp"
#include <cstdio>
#include <fstream>

TEST(ProgArgsTest, InvalidNumberOfArguments) {
  std::vector<std::string> const argv = {"prog", "100", "input.txt"};
  EXPECT_EXIT(ProgArgs(argv.size(), argv), ::testing::ExitedWithCode(255), "Error: Invalid number of arguments: 2.\n");
}

TEST(ProgArgsTest, NonNumericTimeSteps) {
  std::vector<std::string> const argv = {"prog", "abc", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(argv.size(), argv), ::testing::ExitedWithCode(255), "Error: time steps must be numeric.\n");
}

TEST(ProgArgsTest, NegativeTimeSteps) {
  std::vector<std::string> const argv = {"prog", "-100", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(argv.size(), argv), ::testing::ExitedWithCode(254), "Error: Invalid number of time steps.\n");
}

TEST(ProgArgsTest, CannotOpenInputFile) {
  std::vector<std::string> const argv = {"prog", "100", "nonexistent.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(argv.size(), argv), ::testing::ExitedWithCode(253), "Error: Cannot open nonexistent.txt for reading.\n");
}

TEST(ProgArgsTest, CannotOpenOutputFile) {
  // Crear un archivo y establecer sus permisos a solo lectura
  std::ofstream("readonly.txt").close();
  int result = std::system("chmod 444 readonly.txt");   //NOLINT
  if (result != 0) {
    std::cerr << "Error al cambiar los permisos del archivo" << '\n';
  }

  // Crear input.txt si no existe
  std::ofstream const inputFile("input.txt");
  if (!inputFile) {
    std::cerr << "Error: No se pudo crear input.txt.\n";
    exit(errorconst);
  }

  std::vector<std::string> const argv = {"prog", "100", "input.txt", "readonly.txt"};
  EXPECT_EXIT(ProgArgs(argv.size(), argv), ::testing::ExitedWithCode(252), "Error: Cannot open readonly.txt for writing.\n");

  // Limpiar
  result = std::system("rm readonly.txt");  //NOLINT
  if (result != 0) {
    std::cerr << "Error al eliminar el archivo" << '\n';
  }
}

