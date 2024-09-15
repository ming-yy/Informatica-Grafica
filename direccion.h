//*****************************************************************
// File:   direccion.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include "coordenadas.h"
#include "puntoDireccion.h"

class Direccion : public PuntoDireccion {
public:
    Direccion();
    Direccion(float x, float y, float z);
    Direccion(float _coord[3]);
};