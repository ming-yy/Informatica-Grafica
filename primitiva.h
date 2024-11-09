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
    Primitiva() = default;
    
    // Destructor virtual para asegurar que los destructores de las clases derivadas
    // se llamen correctamente.
    virtual ~Primitiva() = default;
    
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve la emisión del objeto en <emision> y si el objeto intersecado es luz o no en
    // <choqueConLuz>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    virtual void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                              RGB& emision, bool& choqueConLuz) const = 0;
    
    virtual Direccion getNormal(const Punto& punto) const = 0;
};
