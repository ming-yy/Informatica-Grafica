//*****************************************************************
// File:   camara.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include "punto.h"
#include "rayo.h"
#include "escena.h"
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

    // Método que devuelve el rayo que va desde la cámara hasta el centro del pixel (ancho, alto)
    Rayo obtenerRayoPixel(unsigned ancho, unsigned alto) const ;
    
    // Método que ...
    void renderizarEscena(unsigned pxlAncho, unsigned pxlAlto, const Escena& escena) const;
    /*  LA FUNCIÓN DEBERÍA SER EXACTAMENTE ESTO
        Falta implementar obtenerRayoPixel y pintar()
     
        
    */
};
