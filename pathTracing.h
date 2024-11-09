//*****************************************************************
// File:   pathTracing.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include "rayo.h"
#include "transformaciones.h"
#include "escena.h"
#include "rgb.h"
#include "camara.h"

// Función que calcula la reflectancia difusa de Lambert.
float calcBrdfDifusa(const float kd);

// Función que calcula el coseno del ángulo de incidencia, es decir, el ángulo formado
// por <n> y <d>. En general, <n> será la normal y <d> la otra dirección.
float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n);

// Función que devuelve False si y solo si no hay ninguna fuente de luz que incide sobre el punto <p0>.
// Además, si devuelve False, la radiancia devuelta será 0.
// En caso contrario, devuelve True. Si devuelve True, también devolverá la radiancia correspondiente
// al punto <p0>.
bool nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                   const float coefDifuso, RGB& radiancia, bool debug);

// Función que calcula ...
void globalizarYNormalizarRayo(Rayo& rayo, const Punto& o, const Direccion& f, const Direccion& u,
                               const Direccion& l);

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


// Función que ...
void luzIndirectaIterativa(const Punto& origenInicial, const Direccion& normalInicial,
                           const Escena& escena, const float kd, const unsigned maxRebotes,
                           RGB& emisionAcumulada, float& brdfCosenoAcumulado, bool debug);

// Función que ...
void recursividadLuzIndirecta(const Punto& origen, const Direccion& normal, const Escena& escena,
                              const float kd, const unsigned rebotesRestantes, RGB& emisionAcumulada,
                              float& brdfCosenoAcumulado, bool debug);

// Función que calcula todas las emisiones y sus radiancias para la escena con la especificación pasada
// por los parámetros y las devuelve dentro de la matriz 2D <coloresEscena>. Los cálculos de emisiones
// son de 1 rayo por pixel.
void renderizarEscena1RPP(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const float kd, const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          std::vector<std::vector<RGB>>& coloresEscena);

// Método que calcula todas las emisiones y sus radiancias para la escena con la especificación pasada
// por los parámetros y las devuelve dentro de la matriz 2D <coloresEscena>. Los cálculos de emisiones
// son de <rpp> rayos por pixel (rpp > 1).
//void renderizarEscenaConAntialising(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
//                              const Escena& escena, float anchoPorPixel, float altoPorPixel,
//                              const float kd, std::vector<std::vector<RGB>>& coloresEscena,
//                              const unsigned rpp);

// Función que ...
void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const std::string& nombreEscena, const unsigned rpp,
                      const float kd, const unsigned maxRebotes, const unsigned numRayosMontecarlo);
