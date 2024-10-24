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

    // Método que devuelve la direccion que hay hacia la esquina superior izquierda
    // en el plano de proyeccion del pixel (ancho, alto)
    Direccion obtenerDireccionEsquinaPixel(unsigned coordAncho, float anchoPorPixel,
                                    unsigned coordAlto, float altoPorPixel) const;

    // Método que devuelve el rayo que va desde la cámara hasta 
    // la esquina superior izquierda del pixel (ancho, alto)
    Rayo obtenerRayoEsquinaPixel(unsigned coordAncho, float anchoPorPixel,
                                    unsigned coordAlto, float altoPorPixel) const;

    // Método que devuelve el rayo que va desde la cámara hasta
    // el centro del pixel (ancho, alto)
    Rayo obtenerRayoCentroPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const;
    // Método que ...
    void renderizarEscena(unsigned numPxlsAncho, unsigned numPxlsAlto, const Escena& escena) const;
    /*  LA FUNCIÓN DEBERÍA SER EXACTAMENTE ESTO
        Falta implementar obtenerRayoPixel y pintar()
     
        
    */
};
