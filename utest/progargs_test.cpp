//#include <gtest/gtest.h>

#include "gtest/gtest.h"
#include "../sim/progargs.hpp"
#include <cstdio>
#include <fstream>


TEST(ProgArgsTest, InvalidNumberOfArguments) {
  std::vector<char*> argv = {"prog", "10", "input.txt"};
  EXPECT_EXIT(ProgArgs(3, argv), ::testing::ExitedWithCode(-1), "Error: Invalid number of arguments: 2.\n");
}

TEST(ProgArgsTest, NonNumericTimeSteps) {
  std::vector<char*> argv = {"prog", "abc", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(-1), "Error: time steps must be numeric.\n");
}

TEST(ProgArgsTest, NegativeTimeSteps) {
  std::vector<char*> argv = {"prog", "-10", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(-2), "Error: Invalid number of time steps.\n");
}

TEST(ProgArgsTest, CannotOpenInputFile) {
  std::vector<char*> argv = {"prog", "10", "non_existent.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(-3), "Error: Cannot open non_existent.txt for reading.\n");
}

TEST(ProgArgsTest, CannotOpenOutputFile) {
  std::vector<char*> argv = {"prog", "10", "input.txt", "/path/to/non_existent_dir/output.txt"};
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(-4), "Error: Cannot open /path/to/non_existent_dir/output.txt for writing.\n");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/*

#include "gtest/gtest.h"
#include "../sim/progargs.hpp"
#include <cstdio>
#include <fstream>

TEST(ProgArgsTest, InvalidNumberOfArguments) {
    const char* argv_const[] = {"prog", "100", "input.txt"};
  char** argv = const_cast<char**>(argv_const);
  EXPECT_EXIT(ProgArgs(3, argv), ::testing::ExitedWithCode(255), "Error: Invalid number of arguments: 2.\n");
}

TEST(ProgArgsTest, NonNumericTimeSteps) {
    const char* argv_const[] = {"prog", "abc", "input.txt", "output.txt"};
  char** argv = const_cast<char**>(argv_const);
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(255), "Error: time steps must be numeric.\n");
}

TEST(ProgArgsTest, NegativeTimeSteps) {
    const char* argv_const[] = {"prog", "-100", "input.txt", "output.txt"};
  char** argv = const_cast<char**>(argv_const);
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(254), "Error: Invalid number of time steps.\n");
}

TEST(ProgArgsTest, CannotOpenInputFile) {
    const char* argv_const[] = {"prog", "100", "nonexistent.txt", "output.txt"};
  char** argv = const_cast<char**>(argv_const);
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(253), "Error: Cannot open nonexistent.txt for reading.\n");
}

TEST(ProgArgsTest, CannotOpenOutputFile) {
  // Crear un archivo y establecer sus permisos a solo lectura
  std::ofstream("readonly.txt").close();
  int result = std::system("chmod 444 readonly.txt");
  if (result != 0) {
    std::cerr << "Error al cambiar los permisos del archivo" << std::endl;
  }

  // Crear input.txt si no existe
  std::ofstream inputFile("input.txt");
  if (!inputFile) {
    std::cerr << "Error: No se pudo crear input.txt.\n";
    exit(-5);
  }
  inputFile.close();

    const char* argv_const[] = {"prog", "100", "input.txt", "readonly.txt"};
  char** argv = const_cast<char**>(argv_const);
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(252), "Error: Cannot open readonly.txt for writing.\n");

  // Limpiar
  result = std::system("rm readonly.txt");
  if (result != 0) {
    std::cerr << "Error al eliminar el archivo" << std::endl;
  }
}
*/