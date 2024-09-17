//*****************************************************************
// File:   direccion.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "direccion.h"

Direccion::Direccion() : PuntoDireccion() {}

Direccion::Direccion(float x, float y, float z) : PuntoDireccion(x, y, z) {}

Direccion::Direccion(std::array<float,3> _coord) : PuntoDireccion(_coord) {}

std::array<float, 4> Direccion::aMatriz() {
    std::array<float, 4> p = {coord[0], coord[1], coord[2], 0.0f};
    return p;
}

Direccion Direccion::operator+(const Direccion& d) const {
    return Direccion(coord[0] + d.coord[0], coord[1] + d.coord[1], coord[2] + d.coord[2]);
}

Direccion Direccion::operator-(const Direccion& d) const {
    return Direccion(coord[0] - d.coord[0], coord[1] - d.coord[1], coord[2] - d.coord[2]);
}

Direccion Direccion::operator*(const float escalar) const {
    return Direccion(coord[0] * escalar, coord[1] * escalar, coord[2] * escalar);
}

Direccion Direccion::operator/(const float escalar) const {
    if (escalar == 0) {
        throw std::invalid_argument("Error: Division por cero no permitida.");
    }
    
    return Direccion(coord[0] / escalar, coord[1] / escalar, coord[2] / escalar);
}

float Direccion::modulo() const {
    return std::sqrt(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);
}

float modulo(const Direccion& d) {
    return d.modulo();
}

Direccion Direccion::normalizar() const {
    float longitud = modulo();
    if (longitud == 0) {
        throw std::invalid_argument(
            "Error: La direccion tiene modulo cero y no se puede calcular el modulo.");
    }
    return *this / longitud;
}

Direccion normalizar(const Direccion& d) {
    return d.normalizar();
}

float Direccion::productoEscalar(const Direccion& d) const {
    return coord[0] * d.coord[0] + coord[1] * d.coord[1] + coord[2] * d.coord[2];
}

float dot(const Direccion& d1, const Direccion& d2) {
    return d1.productoEscalar(d2);
}

Direccion Direccion::productoVectorial(const Direccion& d) const {
    return Direccion(
        coord[1] * d.coord[2] - coord[2] * d.coord[1],
        coord[2] * d.coord[0] - coord[0] * d.coord[2],
        coord[0] * d.coord[1] - coord[1] * d.coord[0]
    );
}

Direccion cross(const Direccion& d1, const Direccion& d2) {
    return d1.productoVectorial(d2);
}
