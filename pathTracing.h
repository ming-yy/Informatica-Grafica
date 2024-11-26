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


// Función que calcula la dirección de la luz reflejada (especular perfecta).
// <wo>: dirección luz entrante. Rayo que ha chocado contra un elemento de la escena
//       en un punto
// <n>:  normal del objeto por el punto donde ha chocado wo.
Direccion calcDirEspecular(const Direccion& wo, const Direccion& n);

// Función que realiza una selección probabilística del tipo de rayo que
// será disparado (difuso, especular o refractante) basándose en los coeficientes
// de la superficie (kd, ks, kt) proporcionados por <coefs>.
int dispararRuletaRusa(const BSDFs& coefs);

// Función que ..
Rayo obtenerRayoRuletaRusa(const int tipoRayo, const Punto& origen, const Direccion& wo,
                           const Direccion& normal, bool debug);

// Función que calcula la reflectancia difusa de Lambert.
RGB calcBrdfDifusa(const RGB &kd);

// Función que calcula la reflectancia especular. Devuelve <ks> porque si se invoca esta
// función, es porque se ha decidido que el rayo va a ser especular.
RGB calcBrdfEspecular(const RGB& ks);

// Función que calcula BSDF dado todos los coeficientes y el tipo de rayo que es.
RGB calcBsdf(const BSDFs& coefs, const int tipoRayo);

// Función que calcula el coseno del ángulo de incidencia, es decir, el ángulo formado
// por <n> y <d>. En general, <n> será la normal y <d> la otra dirección.
float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n);

// Función que devuelve False si y solo si no hay ninguna fuente de luz que incide sobre el punto <p0>.
// Además, si devuelve False, la radiancia devuelta será 0.
// En caso contrario, devuelve True. Si devuelve True, también devolverá la radiancia correspondiente
// al punto <p0>.
// <wi>: dirección luz saliente inversa. Es un rayo hijo del rayo que sale de la cámara, y que
//       interseca con una primita en el punto <p0> y tiene la normal <normal> en ese punto.
//
//
//           CAMBIAR CAMBAIR CAMBIAR
//
//
RGB nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                        const BSDFs& coefs, bool debug);

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

// Función recursiva que calcula la radiancia del punto <origen> y las radiancias de los puntos
// intersectados tras <rebotesRestantes> rebotes
RGB recursividadRadianciaIndirecta(const Punto& origen, const Direccion &wo, const BSDFs &coefsPtoInterseccion, 
                                    const Direccion& normal, const Escena& escena,
                                    const unsigned rebotesRestantes, bool debug);

// Función que, especificaciones contenidas por los parámetros pasados, devuelve la emisión indirecta
// para el punto <ptoIntersec> que tiene la normal <normal> respecto al objeto con el que ha intersecado.
RGB obtenerRadianciaSalienteIndirecta(const Escena& escena, const unsigned maxRebotes, 
                                        const unsigned numRayosMontecarlo, const Punto& ptoIntersec,
                                        const Direccion& wo, const BSDFs &coefsPtoInterseccion,
                                        const Direccion& normal, bool debug);

// Función que devuelve la radiancia saliente total (directa + indirecta) dado un rayo y una escena,
// rebotando un máximo de <maxRebotes> veces y calculada a través de la media de <numRayosMontecarlo> rayos
// Cabe aclarar que el primer punto de intersección es siempre el mismo, los rayos "Montecarlo" comienzan
// aleatoriamente siempre desde este primer punto.
RGB obtenerRadianciaSaliente(const Rayo &rayo, const Escena &escena, const unsigned maxRebotes, 
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
