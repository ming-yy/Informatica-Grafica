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
    Punto centro;   // En UCS
    float radio;
    RGB emision;    // (r,g,b)
    
    
    Esfera();
    Esfera(const Punto& _centro, const float& _radio,
                    const RGB& _emision = RGB({255.0f, 255.0f, 255.0f}));

    Esfera(const Planeta& p);

    
    void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                      RGB& emision) const override;
    
    Direccion getNormal(const Punto& punto) const override;
};
