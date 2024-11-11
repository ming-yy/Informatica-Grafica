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
    bool soyLuz;

    Triangulo();
    Triangulo(const Punto& _v0, const Punto& _v1, const Punto& _v2,
              const RGB& _emision = RGB({255.0f, 255.0f, 255.0f}),
              const bool _soyLuz = false);

    // Método para calcular la intersección entre un rayo y el triángulo
    // Algoritmo usado: Möller–Trumbore
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve la emisión del objeto en <emision> y si el objeto intersecado es luz o no en
    // <choqueConLuz>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                      RGB& emision, bool& choqueConLuz) const override;
    
    // Método que devuelve la normal de la primitiva en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;
    
    // Método que devuelve <True> si y solo si esta primitiva es una fuente de luz.
    bool soyFuenteDeLuz() const override;
    
    // Método que devuelve un punto aleatorio del triángulo en UCS.
    Punto generarPuntoAleatorio() const override;
};
