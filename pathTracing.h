//*****************************************************************
// File:   pathTracing.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include "transformaciones.h"


// Función que dado el vector <normal>, devuelve los vectores <tangente> y <bitangente>. Estos 3
// vectores constituyen una base ortonormal conformada por <normal>, <tangente>, <bitangente>
void construirBaseOrtonormal(const Direccion& normal, Direccion& tangente, Direccion& bitangente);

// Función que devuelve un valor aleatorio para azimut y otro para inclinación
// para muestreo uniforme de ángulo sólido.
void generarAzimutInclinacion(float& azimut, float& inclinacion);

// Función que devuelve las coordenadas cartesianas correspondientes de (azimut, inclinacion)
void getCoordenadasCartesianas(const float azimut, const float inclinacion,
                                float& x, float& y, float& z);


// Función que devuelve una dirección normalizada que tiene como centro
Direccion generarCaminoAleatorio(const Punto& o);
