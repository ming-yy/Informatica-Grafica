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
    Rayo obtenerRayoPixel(unsigned ancho, unsigned alto);
    
    
    /*  LA FUNCIÓN DEBERÍA SER EXACTAMENTE ESTO
        Falta implementar obtenerRayoPixel y pintar()
     
    void renderizarEscena(unsigned pxlAncho, unsigned pxlAlto, const Escena& escena) const {
        for (int ancho = 0; ancho < pxlAncho; ancho++) {
            for (int alto = 0; alto < pxlAlto; alto++) {
                rayo = obtenerRayoPixel(ancho, alto);
                rayo.d = normalizar(rayo.d);
                Base baseLocalToGlobal = Base(this->f, this->l, this->u);
                rayo.d = cambioBase(rayo.d, baseLocalToGlobal, Punto(0.0f, 0.0f, 0.0f), false);
                rayo.o = cambioBase(rayo.o, baseLocalToGlobal, Punto(0.0f, 0.0f, 0.0f), false);
                std::array<float, 3> emision;    // Array para que se cambie su contenido por referencia
                bool interseca = escena.interseccion(rayo, emision);
                if (interseca) {
                    this->pintar(ancho, alto, emision)
                } else {
                    this->pintar(ancho, alto, color negro porque no interseca)
                }
            }
        }
     }
        
    */
};
