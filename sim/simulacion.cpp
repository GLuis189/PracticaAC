#include "../sim/grid.hpp"
// #include "../sim/particle.hpp"
#include "../sim/progargs.hpp"
#include "../sim/variablesglobales.hpp"
#include "../sim/simulacion.hpp"
#include <sstream>

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

double ppm;
int numparticulas;
double masa;
double suavizado;
double pi_sua_6;
double suavizado_2;
int n_x;
int n_y;
int n_z;
double s_x;
double s_y;
double s_z;

grid calcularMalla(std::ifstream& inputfile) {
  ppm           = read_binary_value<float>(inputfile);
  numparticulas = read_binary_value<int>(inputfile);

  if (numparticulas <= 0) {
    std::cerr << "Error: Invalid number of particles: 0.\n";
    exit(errorconst);
  }

  masa      = densidad / (ppm * ppm * ppm);
  suavizado = radio / ppm;
  pi_sua_6 = M_PI * suavizado * suavizado * suavizado * suavizado * suavizado * suavizado;
  suavizado_2 = suavizado * suavizado;

  grid malla(suavizado);
  return malla;
}

std::vector<Particle> leerParticulas(std::ifstream& inputfile, grid & malla) {
  std::vector<Particle> particles;
  particles.reserve(numparticulas);
  int contar_particulas = 0;
  float p_x, p_y, p_z, hvx, hvy, hvz, v_x, v_y, v_z;
  while(read_binary_values(inputfile,  p_x, p_y, p_z, hvx, hvy, hvz, v_x, v_y, v_z)) {
    Particle particle;
    particle.ide = contar_particulas, particle.densidad = 0;
    particle.posicion.c_x = p_x, particle.posicion.c_y = p_y, particle.posicion.c_z = p_z;
    particle.hvelocidad.c_x = hvx, particle.hvelocidad.c_y = hvy, particle.hvelocidad.c_z = hvz;
    particle.velocidad.c_x = v_x, particle.velocidad.c_y = v_y, particle.velocidad.c_z = v_z;
    particle.aceleracion.c_y = -9.8, particle.aceleracion.c_x = 0, particle.aceleracion.c_z = 0;
    particle.calcularBloqueInicial(malla.s_x, malla.s_y, malla.s_z, malla.n_x, malla.n_y, malla.n_z);
    /*ParametrosBloque param;
    param.s_x = s_x;
    param.s_y = s_y;
    param.s_z = s_z;
    param.n_x = valores.n_x;
    param.n_y = valores.n_y;
    param.n_z = valores.n_z;
    particle.calcularBloqueInicial(param);*/
    int const block_key = malla.generarClaveBloque(particle.p_i, particle.p_j, particle.p_k);
    malla.blocks[block_key].addParticle(particle.ide);
    particles.emplace_back(particle);
    ++contar_particulas;
  }
  inputfile.close();
  if (numparticulas != contar_particulas) {
    std::cerr << "Error: Number of particles mismatch. Header:" << numparticulas
              << ", Found:" << contar_particulas << ".\n";
    exit(errorconst);
  }
 return particles;
}

void reposicionar(grid & malla, std::vector<Particle> & particles) {
  for (Particle& particle : particles) {
    //Particle & particle = particles[part];

    particle.Inicializar();

    int const i_anterior = particle.p_i;
    int const j_anterior = particle.p_j;
    int const k_anterior = particle.p_k;

    particle.calcularBloque(malla.s_x,  malla.s_y, malla.s_z, malla.n_x, malla.n_y, malla.n_z);
    /*int clave = malla.generarClaveBloque(particle.i, particle.j, particle.k);
    malla.blocks[clave].addParticle(particle.ide);*/

    if (i_anterior != particle.p_i || j_anterior != particle.p_j || k_anterior != particle.p_k) {
      malla.CambiarBloque(particle.ide, particle.p_i, particle.p_j, particle.p_k, i_anterior, j_anterior, k_anterior);
    }
  }
}

void ResultadosBinarios(std::vector<Particle>& particulas, std::ofstream& outputfile) {
  float ppm2 = static_cast<float>(ppm);
  outputfile.write(reinterpret_cast<const char*>(&ppm2), sizeof(ppm2));
  outputfile.write(reinterpret_cast<const char*>(&numparticulas), sizeof(numparticulas));

  for (const auto& particula : particulas) {
    float values[] = {
      static_cast<float>(particula.posicion.c_x),
      static_cast<float>(particula.posicion.c_y),
      static_cast<float>(particula.posicion.c_z),
      static_cast<float>(particula.hvelocidad.c_x),
      static_cast<float>(particula.hvelocidad.c_y),
      static_cast<float>(particula.hvelocidad.c_z),
      static_cast<float>(particula.velocidad.c_x),
      static_cast<float>(particula.velocidad.c_y),
      static_cast<float>(particula.velocidad.c_z)
    };

    outputfile.write(reinterpret_cast<const char*>(values), sizeof(values));
  }

  outputfile.close();
}

/*void ResultadosBinarios(std::vector<Particle> & particulas,std::ofstream& outputfile){
  float ppm2 = static_cast<float>(ppm);
  outputfile.write(as_buffer(ppm2), sizeof(ppm2));
  outputfile.write(as_buffer(numparticulas), sizeof(numparticulas));
  for (const auto& particula : particulas) {
    // Convertir los valores de doble precisión a precisión simple antes de escribirlos
    float p_x = static_cast<float>(particula.p_x);
    float p_y = static_cast<float>(particula.p_y);
    float p_z = static_cast<float>(particula.p_z);

    float hvx = static_cast<float>(particula.hvx);
    float hvy = static_cast<float>(particula.hvy);
    float hvz = static_cast<float>(particula.hvz);

    float v_x = static_cast<float>(particula.v_x);
    float v_y = static_cast<float>(particula.v_y);
    float v_z = static_cast<float>(particula.v_z);

    // Escribir los valores en el archivo
    outputfile.write(as_buffer(p_x), sizeof(p_x));
    outputfile.write(as_buffer(p_y), sizeof(p_y));
    outputfile.write(as_buffer(p_z), sizeof(p_z));

    outputfile.write(as_buffer(hvx), sizeof(hvx));
    outputfile.write(as_buffer(hvy), sizeof(hvy));
    outputfile.write(as_buffer(hvz), sizeof(hvz));

    outputfile.write(as_buffer(v_x), sizeof(v_x));
    outputfile.write(as_buffer(v_y), sizeof(v_y));
    outputfile.write(as_buffer(v_z), sizeof(v_z));
  }

  outputfile.close();
}*/

void mostrarResultados(std::vector<Particle> & particles, std::ofstream& outputfile){
  for (Particle  const& particle: particles) {
    outputfile << "ID: " << particle.ide << "\n";
    outputfile << "Posición (x, y, z): " << particle.posicion.c_x << ", " << particle.posicion.c_y << ", " << particle.posicion.c_z << "\n";
    outputfile << "Velocidad (vx, vy, vz): " << particle.velocidad.c_x << ", " << particle.velocidad.c_y << ", " << particle.velocidad.c_z << "\n";
    outputfile << "Hvx, Hvy, Hvz: " << particle.hvelocidad.c_x << ", " << particle.hvelocidad.c_y << ", " << particle.hvelocidad.c_z << "\n";
    outputfile << "Densidad: " << particle.densidad << "\n";
    outputfile << "Aceleración (accx, accy, accz): " << particle.aceleracion.c_x << ", " << particle.aceleracion.c_y << ", " << particle.aceleracion.c_z << "\n";
  }
  outputfile.close();
}

void IniciarSimulacion(const ProgArgs& args, std::ofstream& outputfile, grid & malla, std::vector<Particle>& particles){
  for (int time = 0; time < args.nts; time++) {
    if(time>0) {
     reposicionar(malla, particles);
    }
    malla.calcularDensidades(particles, masa, suavizado, suavizado_2);
    malla.calcularAceleraciones(particles, pi_sua_6, masa, suavizado);
    /*malla.ColisionesEjeX_1(particles);
    malla.ColisionesEjeY_1(particles);
    malla.ColisionesEjeZ_1(particles);*/
    for (Particle& particle : particles){
      //Particle & particle = particles[part];

      particle.MoverParticulas(n_x, n_y, n_z);
    }

    /*malla.ColisionesEjeX_2(particles);
    malla.ColisionesEjeY_2(particles);
    malla.ColisionesEjeZ_2(particles);*/
  }

  // Mostrar los datos de las partículas
  ResultadosBinarios(particles, outputfile);
  //mostrarResultados(particles, outputfile);
}


void mostrarDatos(){
  // Mostrar los datos
  std::cout << "Number of particles: " << numparticulas << "\n";
  std::cout << "Particles per meter: " << ppm << "\n";
  std::cout << "Smoothing length: " << suavizado << "\n";
  std::cout << "Particle mass: " << masa << "\n";
  std::cout << "Grid size: " << malla.n_x << " x " << malla.n_y << " x " << malla.n_z << "\n";
  std::cout << "Number of blocks: " << malla.n_x * malla.n_y * malla.n_z << "\n";
  std::cout << "Block size: " << malla.s_x << " x " << malla.s_y << " x " << malla.s_z << "\n";
}

/*std::vector<Particle> leerArchivo(const std::string& nombreArchivo) {
  std::ifstream archivo(nombreArchivo, std::ios::binary);
  std::vector<Particle> particles;

  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return particles;
  }
  float ppm_salida;
  int numparticulas_salida;
  archivo.read(reinterpret_cast<char *>(&ppm_salida), sizeof(ppm_salida));
  archivo.read(reinterpret_cast<char *>(&numparticulas_salida), sizeof(numparticulas_salida));

  while (!archivo.eof()) {
    Particle particle;

    float p_x, py, pz;
    float hvx, hvy, hvz;
    float vx, vy, vz;

    archivo.read(reinterpret_cast<char*>(&p_x), sizeof(p_x));
    archivo.read(reinterpret_cast<char*>(&py), sizeof(py));
    archivo.read(reinterpret_cast<char*>(&pz), sizeof(pz));

    archivo.read(reinterpret_cast<char*>(&hvx), sizeof(hvx));
    archivo.read(reinterpret_cast<char*>(&hvy), sizeof(hvy));
    archivo.read(reinterpret_cast<char*>(&hvz), sizeof(hvz));

    archivo.read(reinterpret_cast<char*>(&vx), sizeof(vx));
    archivo.read(reinterpret_cast<char*>(&vy), sizeof(vy));
    archivo.read(reinterpret_cast<char*>(&vz), sizeof(vz));

    // Convertir los valores de precisión simple a doble precisión
    particle.p_x = static_cast<double>(p_x);
    particle.py = static_cast<double>(py);
    particle.pz = static_cast<double>(pz);

    particle.hvx = static_cast<double>(hvx);
    particle.hvy = static_cast<double>(hvy);
    particle.hvz = static_cast<double>(hvz);

    particle.vx = static_cast<double>(vx);
    particle.vy = static_cast<double>(vy);
    particle.vz = static_cast<double>(vz);

    particles.push_back(particle);
  }

  archivo.close();
  return particles;
}*/

