//*****************************************************************
// File:   plano.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>
#include <vector>
#include "punto.h"
#include "direccion.h"
#include "primitiva.h"
#include "utilidades.h"
#include "rgb.h"


class Plano : public Primitiva {
public:
    Punto centro;        // Centro del plano sin tener en cuenta la distancia <d>
    Punto centroVeradero; // Centro del plano teniendo en cuenta la distancia <d>
    Direccion n;    // Normal respecto al plano (generalmente en UCS)
    Direccion u, v; // Forman una base ortonormal junto con la normal <n>
    float d;        // Distancia respecto del centro
    float minLimite, maxLimite;     // Límites para la luz de área

    
    Plano();
    Plano(const Direccion& _n, const float _d, const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
          const string _material = "difuso", const RGB& _power = RGB(),
          const float _minLimite = -0.5, const float _maxLimite = 0.5,
          const Punto& _c = {0.0f,0.0f,0.0f}, const string rutaTextura = "");

    // Método para calcular la intersección entre un rayo y el plano
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const override;
    
    // Método que devuelve "True" si y solo si el punto <p0> pertecene al plano.
    bool pertenece(const Punto& p0) const override;
    
    // Método que devuelve la normal de la primitiva en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;
    
    // Método que devuelve "True" si y solo si <punto> pertenece al plano
    // y además, es un punto lumínico del plano.
    bool puntoEsFuenteDeLuz(const Punto& punto) const override;
    
    // Método que devuelve un punto aleatorio de la superficie del plano en UCS.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    Punto generarPuntoAleatorio(float& prob) const override;
    
    // Método que obtiene la posición del punto <pto> del plano en el
    // eje U de la textura correspondiente.
    float getEjeTexturaU(const Punto& pto) const override;
    
    // Método que obtiene la posición del punto <pto> del plano en el
    // eje V de la textura correspondiente.
    float getEjeTexturaV(const Punto& pto) const override;

    // Función para mostrar por pantalla la información del plano
    friend ostream& operator<<(ostream& os, const Plano& p);
    
    // Debug
    void diHola() const override;
};
