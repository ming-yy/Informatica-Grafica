//*****************************************************************
// File:   esfera.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************
#pragma once
#include "primitiva.h"
#include "planeta.h"


class Esfera: public Primitiva {
public:
    Punto centro;   // En UCS
    float radio;
    array<float, 3> emision;    // (r,g,b)
    
    
       Esfera();
       Esfera(const Punto& _centro, const float& _radio,
           array<float, 3> _emision = {0.0f, 0.0f, 0.0f});

       Esfera(const Planeta& p);

    
    void interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                      std::array<float, 3>& emision) const override;
};
