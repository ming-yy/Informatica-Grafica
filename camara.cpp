//*****************************************************************
// File:   camara.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "camara.h"
#include "base.h"
#include "transformaciones.h"


Camara::Camara() : o(Punto(0.0f, 0.0f, -3.5f)),
                   l(Direccion(-1.0f, 0.0f, 0.0f)),
                   u(Direccion(0.0f, 1.0f, 0.0f)),
                   f(Direccion(0.0f, 0.0f, 3.0f)) {}


Camara::Camara(std::initializer_list<float> _o, 
               std::initializer_list<float> _l, 
               std::initializer_list<float> _u, 
               std::initializer_list<float> _f) {
    // Lista contiene 3 elementos (x, y, z)
    if (_o.size() == 3 && _l.size() == 3 && _u.size() == 3 && _f.size() == 3) {
        auto it_o = _o.begin();
        o = Punto(*(it_o), *(it_o + 1), *(it_o + 2));

        auto it_l = _l.begin();
        l = Direccion(*(it_l), *(it_l + 1), *(it_l + 2));

        auto it_u = _u.begin();
        u = Direccion(*(it_u), *(it_u + 1), *(it_u + 2));

        auto it_f = _f.begin();
        f = Direccion(*(it_f), *(it_f + 1), *(it_f + 2));
    } else {
        throw std::invalid_argument("Inicializadores deben contener 3 elementos cada uno.");
    }
}


Camara::Camara(Punto& _o, Direccion& _l, Direccion& _u, Direccion& _f)
    : o(_o), l(_l), u(_u), f(_f) {}



Direccion Camara::obtenerDireccionEsquinaPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    float x = modulo(this->f);  // F
    float y = - modulo(this->l) + coordAncho * anchoPorPixel;   // L
    float z = modulo(this->u) - coordAlto * altoPorPixel;       // U

    return Direccion(x,y,z);
}

Rayo Camara::obtenerRayoEsquinaPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {

    Direccion dirEsquina = obtenerDireccionEsquinaPixel(coordAncho, anchoPorPixel, coordAlto, altoPorPixel);

    return Rayo(dirEsquina, Punto(this->o));
}


Rayo Camara::obtenerRayoCentroPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    
    Direccion dirEsquina = obtenerDireccionEsquinaPixel(coordAncho, anchoPorPixel, coordAlto, altoPorPixel);
    Direccion dirCentro = dirEsquina + Direccion(0, anchoPorPixel/2, altoPorPixel/2);

    return Rayo(dirCentro, Punto(this->o));
}

void Camara::renderizarEscena(unsigned numPxlsAncho, unsigned numPxlsAlto, const Escena& escena) const {
    float anchoPorPixel = (modulo(this->l) * 2) / numPxlsAncho;
    float altoPorPixel = (modulo(this->u) * 2) / numPxlsAlto;
    
    for (int ancho = 0; ancho < numPxlsAncho; ancho++) {
        for (int alto = 0; alto < numPxlsAlto; alto++) {
            Rayo rayo = this->obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            rayo.d = normalizar(rayo.d);
            Base baseLocalToGlobal = Base(this->f, this->l, this->u);
            rayo.d = cambioBase(rayo.d, baseLocalToGlobal, Punto(0.0f, 0.0f, 0.0f), false);
            rayo.o = cambioBase(rayo.o, baseLocalToGlobal, Punto(0.0f, 0.0f, 0.0f), false);
            std::array<float, 3> emision;
            bool interseca = escena.interseccion(rayo, emision);
            if (interseca) {
                //this->pintar(ancho, alto, emision);
            } else {
                //this->pintar(ancho, alto, color negro porque no interseca);
            }
        }
    }
 }
