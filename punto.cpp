//*****************************************************************
// File:   punto.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "punto.h"


Punto::Punto() : PuntoDireccion() {}

Punto::Punto(float x, float y, float z) : PuntoDireccion(x, y, z) {}

Punto::Punto(float _coord[3]) : PuntoDireccion(_coord) {}