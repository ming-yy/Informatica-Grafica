//*****************************************************************
// File:   primitiva.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************
#pragma once
#include "utilidades.h"
#include "punto.h"
#include "rayo.h"
#include "rgb.h"
#include "bsdfs.h"
#include "textura.h"
#include <vector>
#include <string>
#include <initializer_list>


// Clase abstracta que todas las primitivas geométricas deben heredar
class Primitiva {
public:
    BSDFs coeficientes;
    RGB power;
    Textura textura;

    Primitiva();
    Primitiva(const RGB& color, const string material, const RGB& _power,
              const string& rutaTextura = "");
    Primitiva(const RGB& color, const array<float, 3> kd, const array<float, 3> ks,
              const array<float, 3> kt, const RGB& _power, const string& rutaTextura = "");
    
    // Destructor virtual para asegurar que los destructores de las clases derivadas
    // se llamen correctamente.
    virtual ~Primitiva() = default;
    
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    virtual void interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const = 0;
    
    // Método virtual que devuelve "True" si y solo si el punto <p0> pertecene a la primitiva.
    virtual bool pertenece(const Punto& p0) const = 0;
    
    // Método virtual que devuelve la normal de la primitiva en el punto <punto>
    virtual Direccion getNormal(const Punto& punto) const = 0;
    
    // Método que devuelve <True> si y solo si esta primitiva es una fuente de luz.
    bool soyFuenteDeLuz() const;
    
    // Método virtual que devuelve "True" si y solo si <punto> pertenece a la primitiva
    // y además, es un punto lumínico de la primitiva.
    virtual bool puntoEsFuenteDeLuz(const Punto& punto) const = 0;
    
    // Método virtual que devuelve un punto aleatorio de la primitiva.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    virtual Punto generarPuntoAleatorio(float& prob) const = 0;
    
    // Método virtual que obtiene la posición del punto <pto> de la primitiva en el
    // eje U de la textura correspondiente.
    virtual float getEjeTexturaU(const Punto& pto) const = 0;
    
    // Método virtual que obtiene la posición del punto <pto> de la primitiva en el
    // eje V de la textura correspondiente.
    virtual float getEjeTexturaV(const Punto& pto) const = 0;
    
    // Método que devuelve "true" si y solo si la primitiva tiene textura.
    bool tengoTextura() const;
    
    // Método que devuelve el coeficiente kd de la primitiva teniendo en cuenta la textura.
    RGB kd_Textura(const Punto& p) const;
    
    // Debug
    virtual void diHola() const = 0;
};
