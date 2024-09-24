//*****************************************************************
// File:   transformaciones.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include <memory>
#include <cmath>
#include <numbers>
#include "matriz.h"
#include "puntoDireccion.h"
#include "punto.h"
#include "base.h"


Matriz<4, 1> translate(const PuntoDireccion& pd, float x, float y, float z);

Matriz<4, 1> scale(const PuntoDireccion& pd, float x, float y, float z);

Matriz<4, 1> rotateX(const PuntoDireccion& pd, float d);

Matriz<4, 1> rotateY(const PuntoDireccion& pd, float d);

Matriz<4, 1> rotateZ(const PuntoDireccion& pd, float d);

// Función que cambia la punto <p> a la base <b> con origen <o> y devuelve el nuevo punto
//Matriz<4, 1> cambioBase(const Punto& p, const Base& b, const Punto& o);
Punto cambioBase(const Punto& p, const Base& b, const Punto& o);

Direccion cambioBase(const Direccion& p, const Base& b, const Punto& o);
