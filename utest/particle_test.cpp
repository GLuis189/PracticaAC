//
// Created by Celia on 18/11/23.
//
#include "../sim/particle.hpp"
#include "../sim/variablesglobales.hpp"

#include "gtest/gtest.h"
#include <cmath>
#include <string>
#include <array>

TEST(ParticleTest, Inicializar) {
  Particle particle;
  particle.Inicializar();
  EXPECT_EQ(particle.densidad, 0) << "La densidad no se inicializó.";
  EXPECT_EQ(particle.aceleracion.c_x, 0) << "La coordenada x de la aceleración no se inicializó.";
  EXPECT_EQ(particle.aceleracion.c_y, g_y) << "La coordenada y de la aceleración no se inicializó.";
  EXPECT_EQ(particle.aceleracion.c_z, 0) << "La coordenada z de la aceleración no se inicializó.";
}

TEST(ParticleTest, CalcularDistancia) {
  Particle particle1; Particle particle2;
  particle1.posicion = {0.0, 0., 0.0};
  particle2.posicion = {1.0, 1.0, 1.0};
  const double mod = std::sqrt(3.0);
  const double expected_dist = mod*mod;
  EXPECT_EQ(particle1.CalcularDistancia(particle2), expected_dist) << "La distancia no coincide con la esperada.";
}

TEST(ParticleTest, VariacionDensidad) {
  const double suav = 10.0;
  Particle particle1; Particle particle2;
  particle1.ide = 1;
  particle2.ide = 2;
  particle1.posicion = {0.0, 0.0, 0.0};
  particle2.posicion = {1.0, 1.0, 1.0};
  particle1.VariacionDensidad(particle2, suav);
  EXPECT_NE(particle1.densidad, 0) << "La densidad de la partícula 1 no se incrementó.";
  EXPECT_NE(particle2.densidad, 0) << "La densidad de la partícula 2 no se incrementó.";
  const double dist = 3.0;    // (sqrt(3.0))^2 = 3.0
  const double d_s = suav-dist;
  const double variacion_densidad_esperada = pow(d_s, 3);
  EXPECT_EQ(particle1.densidad, variacion_densidad_esperada) << "La densidad de la partícula 1 no coincide con la esperada.";
  EXPECT_EQ(particle2.densidad, variacion_densidad_esperada) << "La densidad de la partícula 2 no coincide con la esperada.";
  EXPECT_EQ(particle1.particulas_adyacentes.back(), particle2.ide) << "La partícula 2 no se añadió a las partículas adyacentes de la partícula 1.";
}

const Vector acc = {0, g_y, 0};

TEST(ParticleTest, VariacionAceleracion) {
  Particle particle1; Particle particle2;
  particle1.posicion = {0.0, 0.0, 0.0};
  particle2.posicion = {1.0, 1.0, 1.0};
  particle1.velocidad = {0.0, 0.0, 0.0};
  particle2.velocidad = {1.0, 1.0, 1.0};
  particle1.aceleracion = acc;
  particle1.aceleracion = acc;
  double const suavizado = 1.0;
  double const pi_sua_6 = M_PI * pow(suavizado, n_6);
  double const masa = 1.0;
  particle1.VariacionAcelaracion(particle2, suavizado, pi_sua_6, masa);
  std::array<std::string, 3> coordenadas = {"x", "y", "z"};
  // Comprueba los resultados
  for (int eje = 0; eje < 3; eje++) {
    EXPECT_EQ(particle1.aceleracion[eje], -particle2.aceleracion[eje]) << "Las coordenadas " << coordenadas.at(eje) << " no se han calculado correctamente.";
  }
}

const Vector pos = {-0.0661478, -0.0805976, -0.0648605};
const Vector hvl = {-0.166259, 0.0432823, 0.0442792};
const Vector vel = {-0.191624, 0.0426284, 0.0459564};
const double bmax = 1.0; const double bmin = -1.0;

TEST(ParticleTest, ColisionesEje) {
  Particle particle;
  particle.posicion = pos;
  particle.hvelocidad = hvl;
  particle.velocidad = vel;
  particle.aceleracion = acc;
  Vector aceleracion_esperada = acc;
  for (int eje = 0; eje < 3; eje++) { particle.ColisionesEje(eje, bmax, bmin); }
  for (int eje = 0; eje < 3; eje++) {
    EXPECT_EQ(particle.aceleracion[eje], aceleracion_esperada[eje]) << "La aceleracion no coincide con la esperada";
  }
}

// NOLINTBEGIN
TEST(ParticleTest, ColisionesEje2) {
  Particle particle;
  particle.posicion = pos;
  particle.hvelocidad = hvl;
  particle.velocidad = vel;
  particle.aceleracion = acc;
  Vector pos2 = {-0.0661478, -0.0805976, -0.0648605};
  Vector hvl2 = {-0.166259, 0.0432823, 0.0442792};
  Vector vel2 = {-0.191624, 0.0426284, 0.0459564};
  for (int eje = 0; eje < 3; eje++) { particle.ColisionesEje_2(eje, bmax, bmin); }
  for (int eje = 0; eje < 3; eje++) {
    if (particle.posicion[eje] < bmin) {
      EXPECT_EQ(particle.posicion[eje], bmin) << "La posicion en el eje " << eje << " es menor que bmin";
      EXPECT_EQ(particle.velocidad[eje], -vel2[eje]) << "La velocidad en el eje " << eje << " no se ha invertido correctamente";
      EXPECT_EQ(particle.hvelocidad[eje], -hvl2[eje]) << "La hvelocidad en el eje " << eje << " no se ha invertido correctamente";
    } else if (particle.posicion[eje] > bmax) {
      EXPECT_EQ(particle.posicion[eje], bmax) << "La posicion en el eje " << eje << " es mayor que bmax";
      EXPECT_EQ(particle.velocidad[eje], -vel2[eje]) << "La velocidad en el eje " << eje << " no se ha invertido correctamente";
      EXPECT_EQ(particle.hvelocidad[eje], -hvl2[eje]) << "La hvelocidad en el eje " << eje << " no se ha invertido correctamente";
    } else {
      EXPECT_EQ(particle.posicion[eje], pos2[eje]) << "La posicion en el eje " << eje << " ha cambiado incorrectamente";
      EXPECT_EQ(particle.velocidad[eje], vel2[eje]) << "La velocidad en el eje " << eje << " ha cambiado incorrectamente";
      EXPECT_EQ(particle.hvelocidad[eje], hvl2[eje]) << "La hvelocidad en el eje " << eje << " ha cambiado incorrectamente";
    }
  }
}
//NOLINTEND
const int n_15 = 15;
const int n_21 = 21;
const Vector pos1 = {-0.0367109, -0.08061, -0.0335985};
const Vector vel1 = {0.170606, -0.0339197, 0.0979943};
const Vector hvl1 = {0.166993, -0.0352274, 0.0957909};
const Vector acc1 = {-714.862, 270.884, 769.135};
const double var1 = -0.0372588; const double var2 = -0.0796554; const double var3 = -0.0327335;
const double var4 = -0.190438; const double var5 = -0.115063; const double var6 = 0.480358;
const double var7 = -0.547869; const double var8 = -0.265354; const double var9 = 0.864926;

TEST(ParticleTest, MoverParticula) {
  Particle particle;
  particle.posicion = pos1;
  particle.velocidad = vel1;
  particle.hvelocidad = hvl1;
  particle.aceleracion = acc1;
  particle.MoverParticulas(n_15, n_21, n_15);
  Vector pos_esperada = {var1, var2, var3};
  Vector vel_esperada = {var4, var5, var6};
  Vector hvl_esperada = {var7, var8, var9};
  for (int eje = 0; eje < 3; eje++) {
    EXPECT_NEAR(particle.posicion[eje], pos_esperada[eje], 1e-7) << "La posicion no coincide con la esperada";
    EXPECT_NEAR(particle.velocidad[eje], vel_esperada[eje], 1e-6) << "La velocidad no coincide con la esperada";
    EXPECT_NEAR(particle.hvelocidad[eje], hvl_esperada[eje], 1e-5) << "La hvelocidad no coincide con la esperada";
  }
}