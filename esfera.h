//*****************************************************************
// File:   esfera.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************
#pragma once
#include "primitiva.h"
#include "planeta.h"
#include "rgb.h"


class Esfera: public Primitiva {
public:
    Punto centro;   // Generalmente en UCS
    float radio;
    RGB emision;    // (r,g,b)
    bool soyLuz;
    
    
    Esfera();
    Esfera(const Punto& _centro, const float& _radio,
           const RGB& _emision = RGB({255.0f, 255.0f, 255.0f}), const bool _soyLuz = false);
    Esfera(const Planeta& p);
    
    // Método para calcular la intersección entre un rayo y la esfera
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve la emisión del objeto en <emision> y si el objeto intersecado es luz o no en
    // <choqueConLuz>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                      RGB& emision, bool& choqueConLuz) const override;
    
    Direccion getNormal(const Punto& punto) const override;
};
