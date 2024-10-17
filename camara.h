//*****************************************************************
// File:   camara.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include "punto.h"
#include <initializer_list>


class Camara {
public:
    Punto o;
    Direccion l;
    Direccion u;
    Direccion f;

    
    Camara();
    Camara(init_list<float> _o, init_list<float> _l, init_list<float> _u,
           init_list<float> _f);
    Camara(Punto& _o, Direccion& _l, Direccion& _u, Direccion& _f);

    // size = NxM píxeles,
    /*
    
    for ancho in N:
        for alto in M:
            dir, pto = obtenerDireccionCamPixel(ancho, alto, camara);
            dir = normalizar(dir);
            dirGlobal, ptoGlobal = cambioBaseLocalGlobal(dir, pto);
            interseca, emision = calcInterseccion(escena, dirGlobal, ptoGlobal);
            if Interseca {
                pintar(ancho, alto, emision)
            } else {
                pintar(ancho, alto, color negro porque no interseca)
            }
    
    */
};
