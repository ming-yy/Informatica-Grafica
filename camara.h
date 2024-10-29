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
    Direccion f;
    Direccion l;
    Direccion u;
    

    Camara();
    Camara(Punto& _o, Direccion& _f, Direccion& _l, Direccion& _u);
    Camara(init_list<float> _o, init_list<float> _f, init_list<float> _l,
          init_list<float> _u);

    float calcularAnchoPixel(unsigned numPixeles) const;

    float calcularAltoPixel(unsigned numPixeles) const;

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
    // Método que devuelve el rayo que va desde la cámara hasta
    // un punto aleatorio del pixel (ancho, alto)
    Rayo obtenerRayoAleatorioPixel(unsigned coordAncho, float anchoPorPixel,
                                      unsigned coordAlto, float altoPorPixel) const;

    // Método que ...
    void renderizarEscena(unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, const std::string& nombreEscena, unsigned rpp = 1) const;
    
    // Método que ...
    void renderizarEscenaCentroPixel(unsigned numPxlsAncho, unsigned numPxlsAlto,
                                  const Escena& escena, const std::string& nombreEscena,
                                  float anchoPorPixel, float altoPorPixel,
                                  std::vector<std::vector<RGB>>& coloresEscena) const;
    
    // Método que ...
    void renderizarEscenaConAntialising(unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, const std::string& nombreEscena,
                              float anchoPorPixel, float altoPorPixel,
                              std::vector<std::vector<RGB>>& coloresEscena, unsigned rpp) const;
};


// Función que calcula ...
void globalizarYNormalizarRayo(Rayo& rayo, const Punto& o, const Direccion& f, const Direccion& u,
                               const Direccion& l);
