//*****************************************************************
// File:   camara.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "camara.h"
#include "rgb.h"
#include "base.h"
#include "transformaciones.h"


Camara::Camara() : o(Punto(0.0f, 0.0f, -3.5f)),
                   f(Direccion(-1.0f, 0.0f, 0.0f)),
                   l(Direccion(0.0f, 1.0f, 0.0f)),
                   u(Direccion(0.0f, 0.0f, 3.0f)) {}


Camara::Camara(std::initializer_list<float> _o, 
               std::initializer_list<float> _f, 
               std::initializer_list<float> _l, 
               std::initializer_list<float> _u) {
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


Camara::Camara(Punto& _o, Direccion& _f, Direccion& _l, Direccion& _u)
    : o(_o), l(_l), u(_u), f(_f) {}


Direccion Camara::obtenerDireccionEsquinaPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    float x = modulo(this->f);
    float y = - modulo(this->l) + coordAncho * anchoPorPixel;
    float z = modulo(this->u) - coordAlto * altoPorPixel;
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

float Camara::calcularAnchoPixel(unsigned numPixeles) const {
    return (modulo(this->l) * 2) / numPixeles;
}

float Camara::calcularAltoPixel(unsigned numPixeles) const{
    return (modulo(this->u) * 2) / numPixeles;
}


void Camara::renderizarEscena(unsigned numPxlsAncho, unsigned numPxlsAlto, const Escena& escena) const {
    float anchoPorPixel = this->calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = this->calcularAltoPixel(numPxlsAlto);
    /*
    std:array<array<RGB, numPxlsAlto>, numPxlsAncho> coloresEscena;
    for (auto& fila : coloresEscena) {      // Inicializamos con todo negro
        fila.fill(255.0f);
    }
    */

    for (int ancho = 0; ancho < numPxlsAncho; ancho++) {
        for (int alto = 0; alto < numPxlsAlto; alto++) {
            Rayo rayo = this->obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            rayo.d = normalizar(rayo.d);
            Base baseLocalToGlobal = Base(this->f, this->l, this->u);
            rayo.d = cambioBase(rayo.d, baseLocalToGlobal, Punto(0.0f, 0.0f, 0.0f), false);
            rayo.o = cambioBase(rayo.o, baseLocalToGlobal, Punto(0.0f, 0.0f, 0.0f), false);
            RGB emision;
            bool interseca = escena.interseccion(rayo, emision);
            if (interseca) {
                // coloresEscena[alto][ancho] = emision;
            }
        }
        // pintar();
    }
 }
