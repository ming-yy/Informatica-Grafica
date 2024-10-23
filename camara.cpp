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


Rayo Camara::obtenerRayoPixel(unsigned ancho, unsigned alto) const {
    float i = modulo(this->f);
    float j = 0.0f;    // L
    float k = 0.0f;    // U
    
    // No me sé las fórmulas que acordamos
    
    return Rayo(Direccion(i, j, k), Punto(this->o));  // Falta completar la direccion y el punto
}


void Camara::renderizarEscena(unsigned pxlAncho, unsigned pxlAlto, const Escena& escena) const {
    for (int ancho = 0; ancho < pxlAncho; ancho++) {
        for (int alto = 0; alto < pxlAlto; alto++) {
            Rayo rayo = this->obtenerRayoPixel(ancho, alto);
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
