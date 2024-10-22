//*****************************************************************
// File:   triangulo.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include "punto.h"
#include "direccion.h"
#include <iostream>
#include <cmath>


class Triangulo {
public:
    Punto v0, v1, v2;
    array<float, 3> emision;

    Triangulo();
    Triangulo(const Punto& _v0,const Punto& _v1, const Punto& _v2,
              array<float, 3> _emision = {0.0f, 0.0f, 0.0f});

    // Método para calcular la intersección entre un rayo y el triángulo
    // Devuelve <True> si hay intersección y almacena el punto de intersección
    // en la variable 'puntoInterseccion'
    // Algoritmo usado: Möller–Trumbore
    bool interseccionRayoTriangulo(Punto& origen, Direccion& direccion, Punto& puntoInterseccion);
};
