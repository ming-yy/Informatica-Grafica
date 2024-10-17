//*****************************************************************
// File:   plano.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>
#include "punto.h"
#include "direccion.h"


class Plano {
public:
    Punto c;        // Centro del plano
    Direccion n;    // Normal respecto al plano
    float d;        // Distancia máxima admitida
    array<float, 3> emision;     // (r,g,b)
    
    Plano();
    Plano(Punto& _c, Direccion& _n, float _d, array<float,3> _emision);
    
    // Función que calcula la intersección entre un rayo (d) con origen
    // en p y el plano e. Devuelve <True> si y solo si se produce la
    // interseccion.
    friend bool interseccionRayoPlano(Punto& p, Direccion& d, Plano& e,
                                      Punto& puntoInterseccion);
    
    // Función para mostrar por pantalla la información del plano
    friend ostream& operator<<(ostream& os, const Plano& p);
};
