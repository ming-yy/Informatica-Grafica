//*****************************************************************
// File:   primitiva.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************
#pragma once
#include "punto.h"
#include "rayo.h"
#include "rgb.h"
#include <vector>

// Clase virtual que todas las primitivas geométricas deben heredar
class Primitiva {
public:
    Primitiva();
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. También devuelve la emisión del
    // objeto en "emision".
    virtual void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                              RGB& emision) const = 0;
};
