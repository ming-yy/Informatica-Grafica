//*****************************************************************
// File:   punto.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "punto.h"


Punto::Punto() : PuntoDireccion(), o{0.0f, 0.0f, 0.0f} {}

Punto::Punto(float x, float y, float z) : PuntoDireccion(x, y, z), o{0.0f, 0.0f, 0.0f}{}

Punto::Punto(float _coord[3]) : PuntoDireccion(_coord), o{0.0f, 0.0f, 0.0f}{}

Punto Punto::operator+(const Direccion& d) const {
    return Punto(coord[0] + d.coord[0], coord[1] + d.coord[1], coord[2] + d.coord[2]);
}

Punto Punto::operator-(const Direccion& d) const {
    return Punto(coord[0] - d.coord[0], coord[1] - d.coord[1], coord[2] - d.coord[2]);
}

Punto Punto::operator*(const float escalar) const {
    return Punto(coord[0] * escalar, coord[1] * escalar, coord[2] * escalar);
}

Punto Punto::operator/(const float escalar) const {
    if (escalar == 0) {
        throw std::invalid_argument("Error: División por cero no permitida.");
    }
    
    return Punto(coord[0] / escalar, coord[1] / escalar, coord[2] / escalar);
}
