//*****************************************************************
// File:   punto.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include "puntoDireccion.h"


class Punto : public PuntoDireccion {
public:
    //float o[] = {0.0f, 0.0f, 0.0f};     
    
    Punto();
    Punto(float x, float y, float z);
    Punto(float _coord[3]);
};