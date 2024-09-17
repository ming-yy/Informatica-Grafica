//*****************************************************************
// File:   coordenadas.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>


class Coordenadas {
public:
    float coord[3];
    
    Coordenadas();
    Coordenadas(float x, float y, float z);
    Coordenadas(std::array<float, 3> _coord);

    friend std::ostream& operator<<(std::ostream& os, const Coordenadas& pd);

    /*
    operator +
    operator -
    operator *
    operator /
    
    */
};