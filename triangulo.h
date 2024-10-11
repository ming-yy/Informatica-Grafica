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

    Triangulo();
    Triangulo(Punto& _v0, Punto& _v1, Punto& _v2);

    // Método para calcular la intersección entre un rayo y el triángulo
    // Devuelve <True> si hay intersección y almacena el punto de intersección
    // en la variable 'puntoInterseccion'
    // Algoritmo usado: Möller–Trumbore
    bool interseccionRayoTriangulo(Punto& origen, Direccion& direccion, Punto& puntoInterseccion);
};
