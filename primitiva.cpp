//*****************************************************************
// File:   primitiva.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "primitiva.h"
#include "bsdfs.h"
#include <stdexcept>
#include <array>


Primitiva::Primitiva(): coeficientes(BSDFs()), power(RGB()), textura(Textura()) {}

Primitiva::Primitiva(const RGB& color, const string material, const RGB& _power, const string& rutaTextura):
                     coeficientes(color, material), power(_power), textura(Textura(rutaTextura)) {}

Primitiva::Primitiva(const RGB& color, const array<float, 3> kd, const array<float, 3> ks,
                     const array<float, 3> kt, const RGB& _power, const string& rutaTextura):
                     coeficientes(color, kd, ks, kt), power(_power), textura(Textura(rutaTextura)) {}

bool Primitiva::soyFuenteDeLuz() const {
    return !valeCero(this->power);
}

bool Primitiva::tengoTextura() const {
    return !((this->textura.alto == 0) || (this->textura.ancho == 0));
}

RGB Primitiva::kd_Textura(const Punto& p) const {
    //int x = floor(this->textura.ancho * this->getEjeTexturaU(p)) + 1;
    float y = this->getEjeTexturaU(p);
    //int y = floor(this->textura.alto * this->getEjeTexturaV(p)) + 1;
    float x = this->getEjeTexturaV(p);
    return this->textura.obtenerTextura(x, y) * this->coeficientes.sinEmision[KD_i];
}
