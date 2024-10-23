//*****************************************************************
// File:   primitiva.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "punto.h"
#include "rayo.h"

// Clase virtual que todas las primitivas geométrica deben heredar
// Quizás también habría que hacer que intersección() sea virtual

class Primitiva {
public:
    Primitiva();
    
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. También devuelve la emisión del
    // objeto en emisión.
    virtual void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                         std::array<float, 3>& emision) const;
};
