//*****************************************************************
// File:   puntoDireccion.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>


class PuntoDireccion {
public:
    float coord[3];
    
    PuntoDireccion();
    PuntoDireccion(float x, float y, float z);
    PuntoDireccion(float _coord[3]);

    friend std::ostream& operator<<(std::ostream& os, const PuntoDireccion& pd);

    /*
    operator +
    operator -
    operator *
    operator /
    
    */
};