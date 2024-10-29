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
#include "primitiva.h"
#include "rayo.h"
#include "rgb.h"


class Triangulo : public Primitiva {
public:
    Punto v0, v1, v2;
    RGB emision;

    Triangulo();
    Triangulo(const Punto& _v0, const Punto& _v1, const Punto& _v2,
              const RGB& _emision = RGB({255.0f, 255.0f, 255.0f}));

    // Método para calcular la intersección entre un rayo y el triángulo
    // Devuelve <True> si hay intersección y almacena el punto de intersección
    // en la variable 'puntoInterseccion'
    // Algoritmo usado: Möller–Trumbore
    void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                      RGB& emision) const override;
    
    Direccion getNormal(const Punto& punto) const override;
};
