//*****************************************************************
// File:   punto.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include "puntoDireccion.h"
#include "direccion.h"
#include "matriz.h"

class Punto : public PuntoDireccion {
public:
    std::array<float, 3> o;

    Punto();
    Punto(float x, float y, float z);
    Punto(std::array<float, 3> _coord);
    Matriz<4,1> getCoordHomo() const override;
    
    // Operación de suma de un punto con una dirección
    Punto operator+(const Direccion& d) const;
    
    // Operación de resta de un punto con una dirección
    Punto operator-(const Direccion& d) const;
    
    // Operación de multiplicación entre un escalar y el punto
    Punto operator*(const float escalar) const;
    
    // Operación de división entre un escalar y el punto
    Punto operator/(const float escalar) const;
};
