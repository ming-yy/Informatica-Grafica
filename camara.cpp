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
#include "gestorPPM.h"
#include <iostream>
#include <random>


Camara::Camara() : o(Punto(0.0f, 0.0f, -3.5f)),
                   f(Direccion(-1.0f, 0.0f, 0.0f)),
                   u(Direccion(0.0f, 0.0f, 3.0f)),
                   l(Direccion(0.0f, 1.0f, 0.0f)) {}


Camara::Camara(std::initializer_list<float> _o, 
               std::initializer_list<float> _f, 
               std::initializer_list<float> _u, 
               std::initializer_list<float> _l) {
    // Lista contiene 3 elementos (x, y, z)
    if (_o.size() == 3 && _l.size() == 3 && _u.size() == 3 && _f.size() == 3) {
        auto it_o = _o.begin();
        o = Punto(*(it_o), *(it_o + 1), *(it_o + 2));
        
        auto it_f = _f.begin();
        f = Direccion(*(it_f), *(it_f + 1), *(it_f + 2));

        auto it_u = _u.begin();
        u = Direccion(*(it_u), *(it_u + 1), *(it_u + 2));
        
        auto it_l = _l.begin();
        l = Direccion(*(it_l), *(it_l + 1), *(it_l + 2));

    } else {
        throw std::invalid_argument("Inicializadores deben contener 3 elementos cada uno.");
    }
}


Camara::Camara(Punto& _o, Direccion& _f, Direccion& _u, Direccion& _l)
    : o(_o), l(_l), u(_u), f(_f) {}


Direccion Camara::obtenerDireccionEsquinaPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    float x = modulo(this->f);
    float y = modulo(this->u) - coordAlto * altoPorPixel;
    float z = - modulo(this->l) + coordAncho * anchoPorPixel;
    return Direccion(x,y,z);
}


Rayo Camara::obtenerRayoEsquinaPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    Direccion dirEsquina = obtenerDireccionEsquinaPixel(coordAncho, anchoPorPixel, coordAlto, altoPorPixel);
    return Rayo(dirEsquina, Punto(0.0f, 0.0f, 0.0f));
}


Rayo Camara::obtenerRayoCentroPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    Direccion dirEsquina = obtenerDireccionEsquinaPixel(coordAncho, anchoPorPixel, coordAlto, altoPorPixel);
    Direccion dirCentro = dirEsquina + Direccion(0,  - altoPorPixel/2, anchoPorPixel/2);
    return Rayo(dirCentro, Punto(0.0f, 0.0f, 0.0f));
}

Rayo Camara::obtenerRayoAleatorioPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    std::random_device rd;  // Dispositivo para obtener una semilla (aleatoriedad real del hardware)
    std::mt19937 gen(rd()); // Motor de generación Mersenne Twister con semilla
    std::uniform_real_distribution<> distribucion(0.0, 1.0);  // Rango entre 0.0 y 1.0

    Direccion dirEsquina = obtenerDireccionEsquinaPixel(coordAncho, anchoPorPixel, coordAlto, altoPorPixel);
    
    // Multiplicamos el float aleatorio (0, 1) generado por el ancho/alto del pixel
    // y se lo sumamos a la esquina para obtener las nuevas coordenadas aleatorias
    float anchoRand = distribucion(gen) * anchoPorPixel;
    float altoRand = distribucion(gen) * altoPorPixel;

    Direccion dirRand = dirEsquina + Direccion(0, anchoRand, - altoRand);
    return Rayo(dirRand, Punto(0.0f, 0.0f, 0.0f));
}

Rayo Camara::obtenerRppRayosAleatoriosPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel, unsigned rpp) const {
    Rayo rayoMedio(Direccion(0.0f, 0.0f, 0.0f), Punto());

    for(int i = 0; i < rpp; i++){
        rayoMedio = Rayo(rayoMedio.d + obtenerRayoAleatorioPixel(coordAncho, anchoPorPixel, 
                                                                    coordAlto, altoPorPixel).d,
                            Punto(0.0f, 0.0f, 0.0f));
    }

    rayoMedio = Rayo(rayoMedio.d / rpp, Punto(0.0f, 0.0f, 0.0f));

    return rayoMedio;
}

float Camara::calcularAnchoPixel(unsigned numPixeles) const {
    return (modulo(this->l) * 2) / numPixeles;
}

float Camara::calcularAltoPixel(unsigned numPixeles) const{
    return (modulo(this->u) * 2) / numPixeles;
}



void imprimirImagen(const std::vector<std::vector<RGB>>& imagen) {
    for (const auto& fila : imagen) {
        for (const auto& pixel : fila) {
            std::cout << "(" << pixel.rgb[0] << ", " << pixel.rgb[1] << ", " << pixel.rgb[2] << ") ";
        }
        std::cout << std::endl; // Nueva línea para cada fila
    }
}

void Camara::renderizarEscena(unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, const std::string& nombreEscena, unsigned rpp = 1) const {
    if (rpp < 1) {
        std::cout << "RPP tiene que ser mayor que 0" << endl;
        return;
    }

    float anchoPorPixel = this->calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = this->calcularAltoPixel(numPxlsAlto);

    std::vector<std::vector<RGB>> coloresEscena(numPxlsAlto,
                                                std::vector<RGB>(numPxlsAncho, {0.0f, 0.0f, 0.0f}));

    bool useCenterRay = (rpp == 1);

    for (unsigned ancho = 0; ancho < numPxlsAncho; ancho++) {
        for (unsigned alto = 0; alto < numPxlsAlto; alto++) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());

            if(useCenterRay){
                rayo = this->obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            } else {
                rayo = this->obtenerRppRayosAleatoriosPixel(ancho, anchoPorPixel, alto, altoPorPixel, rpp);
            }
            //Rayo rayo = this->obtenerRayoEsquinaPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            rayo.d = normalizar(rayo.d);
            //Base baseLocalToGlobal = Base(this->f, this->u, this->l);   // Pasamos rayo a UCS
            Base baseLocalToGlobal = Base({{0.0f,0.0f,1.0f},
                                            {0.0f,1.0f,0.0f},
                                            {1.0f,0.0f,0.0f}});
            
            // if(ancho == 128 && alto == 128) {
            //     std::cout << rayo << endl;
            //     std::cout << baseLocalToGlobal << endl;
            // }

            rayo.d = cambioBase(rayo.d, baseLocalToGlobal, this->o, false);
            rayo.o = cambioBase(rayo.o, baseLocalToGlobal, this->o, false);

            // if(ancho == 128 && alto == 128) {
            //     std::cout << rayo << endl;
            //     std::cout << baseLocalToGlobal << endl;
            // }

            rayo.d = normalizar(rayo.d);

            RGB emision;
            bool interseca = escena.interseccion(rayo, emision);
            
            if (interseca) {    // Si el rayo interseca con un objeto de la escena
                //std::cout << "interseca" << endl;
                coloresEscena[alto][ancho] = emision;
            }
        }
    }
    //imprimirImagen(coloresEscena);
    pintarEscenaEnPPM(nombreEscena, 255.0f, 1.0f, coloresEscena);
}
