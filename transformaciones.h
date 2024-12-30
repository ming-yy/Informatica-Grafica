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
#include "punto.h"
#include "base.h"
#include "direccion.h"

Punto translate(const Punto& pd, float x, float y, float z);

Punto scale(const Punto& pd, float x, float y, float z);

Punto rotateX(const Punto& pd, float d);

Punto rotateY(const Punto& pd, float d);

Punto rotateZ(const Punto& pd, float d);

// Función que cambia el punto <p> a la base <b> con origen <o> y devuelve el nuevo punto
Punto cambioBase(const Punto& p, const Base& b, const Punto& o, const bool& invertir = true);

// Función que cambia la direccion <d> a la base <b> con origen <o> y devuelve el nuevo punto
Direccion cambioBase(const Direccion& d, const Base& b, const Punto& o, const bool& invertir = true);

Punto puntoFromCoordHomo(Matriz<4,1> m);

Direccion dirFromCoordHomo(Matriz<4,1> m);
