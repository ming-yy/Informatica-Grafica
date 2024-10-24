//*****************************************************************
// File:   plano.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>
#include <vector>
#include "punto.h"
#include "direccion.h"
#include "primitiva.h"
#include "rgb.h"


class Plano : public Primitiva {
public:
    Punto c;        // Centro del plano
    Direccion n;    // Normal respecto al plano
    float d;        // Distancia máxima admitida
    RGB emision;     // (r,g,b)
    
    Plano();
    Plano(const Direccion& _n, float _d, const RGB& _emision = RGB({255.0f, 255.0f, 255.0f}));
    
    // Función que calcula la intersección entre un rayo (d) con origen
    // en p y el plano e. Devuelve <True> si y solo si se produce la
    // interseccion.
    void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                      RGB& emision) const override;
    
    // Función para mostrar por pantalla la información del plano
    friend ostream& operator<<(ostream& os, const Plano& p);
};
