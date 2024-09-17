//*****************************************************************
// File:   direccion.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include "puntoDireccion.h"
#include <cmath>
#include <stdexcept>  // Para manejar excepciones
#include "matriz.h"


class Direccion : public PuntoDireccion {
public:
    Direccion();
    Direccion(float x, float y, float z);
    Direccion(std::array<float,3> _coord);
    Matriz<4,1> aMatriz4x1() override;

    
    // Operación de suma de dos direcciones
    Direccion operator+(const Direccion& d) const;
    
    // Operación de resta de dos direcciones
    Direccion operator-(const Direccion& d) const;
    
    // Operación de multiplicación entre un escalar y la dirección
    Direccion operator*(const float escalar) const;
    
    // Operación de división entre un escalar y la dirección
    Direccion operator/(const float escalar) const;
    
    // Función global para calcular el módulo de la dirección
    friend float modulo(const Direccion& d);
    
    // Función global para normalizar la dirección
    friend Direccion normalizar(const Direccion& d);

    // Función global para producto escalar entre dos direcciones
    friend float dot(const Direccion& d1, const Direccion& d2);
    
    // Función global para producto vectorial entre dos direcciones
    friend Direccion cross(const Direccion& d1, const Direccion& d2);
    
private:
    // Método privado para calcular el módulo de la dirección
    float modulo() const;
    
    // Método privado para normalizar la dirección
    Direccion normalizar() const;
    
    // Método privado para producto escalar con otra dirección
    float productoEscalar(const Direccion& d) const;

    // Método privado para producto vectorial con otra dirección
    Direccion productoVectorial(const Direccion& d) const;
};
