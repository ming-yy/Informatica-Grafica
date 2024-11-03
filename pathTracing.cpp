//*****************************************************************
// File:   pathTracing.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "pathTracing.h"
#include <cmath>
#include <random>

//const double M_PI = 3.14159265358979323846;   // Por si no va cmath
#define GRAD_A_RAD 3.1415926535898f/180


void construirBaseOrtonormal(const Direccion& normal, Direccion& tangente, Direccion& bitangente) {
    if (fabs(normal.coord[0]) > fabs(normal.coord[2])) {
        tangente = Direccion(-normal.coord[1], normal.coord[0], 0);
    } else {
        tangente = Direccion(0, -normal.coord[2], normal.coord[1]);
    }
    tangente = normalizar(tangente);
    bitangente = cross(normal, tangente);
}

void generarAzimutInclinacion(float& azimut, float& inclinacion) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    inclinacion = acos(sqrt(1-dis(gen)));
    azimut = 2 * M_PI * dis(gen);
}

void getCoordenadasCartesianas(const float azimut, const float inclinacion,
                                float& x, float& y, float& z) {
    float sinAzim = static_cast<float>(sin(float(azimut * GRAD_A_RAD)));
    float sinIncl = static_cast<float>(sin(float(inclinacion * GRAD_A_RAD)));
    float cosAzim = static_cast<float>(cos(float(azimut * GRAD_A_RAD)));
    float cosIncl = static_cast<float>(cos(float(inclinacion * GRAD_A_RAD)));
    x = sinIncl * cosAzim;
    y = sinIncl * sinAzim;
    z = cosIncl;
}

Direccion generarCaminoAleatorio(const Punto& o, const Direccion& normal) {
    float inclinacion, azimut;
    float x, y, z;

    generarAzimutInclinacion(azimut, inclinacion);
    getCoordenadasCartesianas(azimut, inclinacion, x, y, z);
    Direccion wi_local = normalizar(Direccion(x, y, z));      // Inclinacion positiva, hemisferio norte asegurado
    
    Direccion tangente;
    Direccion bitangente;
    construirBaseOrtonormal(normal, tangente, bitangente);
    return normalizar(tangente * wi_local.coord[0] +
                      bitangente * wi_local.coord[1] +
                      normal * wi_local.coord[2]);
}
