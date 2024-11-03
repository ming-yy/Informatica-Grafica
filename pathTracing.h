//*****************************************************************
// File:   pathTracing.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 4 de Informática Gráfica
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


// Función que devuelve un rayo generado aleatoriamente. El origen del rayo es <o> y la dirección
// del rayo es una aleatoria pero está contenida en el hipotético hemisferio superior que tiene
// como centro al punto <o> y como altura a la dirección <normal> (|normal| == radio hemisferio).
Rayo generarCaminoAleatorio(const Punto& o, const Direccion& normal);
