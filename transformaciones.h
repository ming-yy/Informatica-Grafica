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


Matriz<4, 1> translate(const std::shared_ptr<PuntoDireccion>& pd, float x, float y, float z);

Matriz<4, 1> scale(const std::shared_ptr<PuntoDireccion>& pd, float x, float y, float z);

Matriz<4, 1> rotateX(const std::shared_ptr<PuntoDireccion>& pd, float d);

Matriz<4, 1> rotateY(const std::shared_ptr<PuntoDireccion>& pd, float d);

Matriz<4, 1> rotateZ(const std::shared_ptr<PuntoDireccion>& pd, float d);

// Función que cambia la punto <p> a la base <b> con origen <o> y devuelve el nuevo punto
std::array<float, 3> cambioBase(const std::shared_ptr<Punto>& p, const Base b,
                 const std::shared_ptr<Punto>& o);
