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
#include "utilidades.h"


// Función que calcula la reflectancia difusa de Lambert.
RGB calcBrdfDifusa(const RGB &kd);

// Función que calcula el coseno del ángulo de incidencia, es decir, el ángulo formado
// por <n> y <d>. En general, <n> será la normal y <d> la otra dirección.
float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n);

// Función que devuelve False si y solo si no hay ninguna fuente de luz que incide sobre el punto <p0>.
// Además, si devuelve False, la radiancia devuelta será 0.
// En caso contrario, devuelve True. Si devuelve True, también devolverá la radiancia correspondiente
// al punto <p0>.
RGB nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                         const RGB &kd, bool debug);

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


// Función iterativa que calcula la radiancia del punto <origenInicial> y las radiancias de los puntos
// intersectados tras <rebotesRestantes> rebotes
void iterativaRadianciaIndirecta(const Punto& origenInicial, const Direccion& normalInicial,
                           const Escena& escena, const unsigned maxRebotes,
                           RGB& emisionAcumulada, float& brdfCosenoAcumulado, bool debug);

// Función recursiva que calcula la radiancia del punto <origen> y las radiancias de los puntos
// intersectados tras <rebotesRestantes> rebotes
RGB recursividadRadianciaIndirecta(const Punto& origen, const Direccion& normal, const Escena& escena,
                             const unsigned rebotesRestantes, bool debug);

// Función que, especificaciones contenidas por los parámetros pasados, devuelve la emisión indirecta
// para el punto <ptoIntersec> que tiene la normal <normal> respecto al objeto con el que ha intersecado.
RGB obtenerRadianciaSalienteIndirecta(const Escena& escena, const unsigned maxRebotes,
                            const unsigned numRayosMontecarlo, const Punto& ptoIntersec, const Direccion& normal,
                            bool debug);

// Función que devuelve la radiancia saliente total (directa + indirecta) dado un rayo y una escena,
// rebotando un máximo de <maxRebotes> veces y calculada a través de la media de <numRayosMontecarlo> rayos
// Cabe aclarar que el primer punto de intersección es siempre el mismo, los rayos "Montecarlo" comienzan
// aleatoriamente siempre desde este primer punto.
RGB obtenerRadianciaSaliente(Rayo &rayo, const Escena &escena, const unsigned maxRebotes, 
                                const unsigned numRayosMontecarlo, bool debug);

// Método que muestra por pantalla el número de píxeles procesados (cada 100 píxeles)
void printPixelActual(unsigned totalPixeles, unsigned numPxlsAncho, unsigned ancho, unsigned alto);

// Función que calcula todas las emisiones y sus radiancias para la escena con la especificación pasada
// por los parámetros y las devuelve dentro de la matriz 2D <coloresEscena>. Los cálculos de emisiones
// son de 1 rayo por pixel.
void renderizarEscena1RPP(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const unsigned totalPixeles,
                          const bool printPixelesProcesados);

// Método que calcula todas las emisiones y sus radiancias para la escena con la especificación pasada
// por los parámetros y las devuelve dentro de la matriz 2D <coloresEscena>. Los cálculos de emisiones
// son de <rpp> rayos por pixel (rpp > 1).
void renderizarEscenaConAntialiasing(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const bool printPixelesProcesados,
                          const unsigned totalPixeles, const unsigned rpp);

// Función que ...
void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const string& nombreEscena, const unsigned rpp,
                      const unsigned maxRebotes, const unsigned numRayosMontecarlo, const bool printPixelesProcesados);
