
//#include <gtest/gtest.h>

#include "gtest/gtest.h"
#include "../sim/progargs.hpp"
#include <cstdio>

TEST(ProgArgsTest, InvalidNumberOfArguments) {
  char* argv[] = {"prog", "100", "input.txt"};
  EXPECT_EXIT(ProgArgs(3, argv), ::testing::ExitedWithCode(-1), "Error: Invalid number of arguments: 2.\n");
}

TEST(ProgArgsTest, NonNumericTimeSteps) {
  char* argv[] = {"prog", "abc", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(-1), "Error: time steps must be numeric.\n");
}

TEST(ProgArgsTest, NegativeTimeSteps) {
  char* argv[] = {"prog", "-100", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(-2), "Error: Invalid number of time steps.\n");
}

TEST(ProgArgsTest, CannotOpenInputFile) {
  char* argv[] = {"prog", "100", "nonexistent.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(-3), "Error: Cannot open nonexistent.txt for reading.\n");
}

TEST(ProgArgsTest, CannotOpenOutputFile) {
  // Crear un archivo y establecer sus permisos a solo lectura
  std::ofstream("readonly.txt").close();
  std::system("chmod 444 readonly.txt");

  char* argv[] = {"prog", "100", "input.txt", "readonly.txt"};
  EXPECT_EXIT(ProgArgs(4, argv), ::testing::ExitedWithCode(-4), "Error: Cannot open readonly.txt for writing.\n");

  // Limpiar
  std::system("rm readonly.txt");
}