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
//#include <cmath>     // permite usar número pi

const double PI = 3.14159265358979323846;

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

void globalizarYNormalizarRayo(Rayo& rayo, const Punto& o, const Direccion& f, const Direccion& u,
                               const Direccion& l) {
    //Rayo rayo = this->obtenerRayoEsquinaPixel(ancho, anchoPorPixel, alto, altoPorPixel);
    rayo.d = normalizar(rayo.d);
    Base baseLocalToGlobal = Base(abs(f / modulo(f)),
                                  abs(u / modulo(u)),
                                  abs(l / modulo(l)));
    
    rayo.d = cambioBase(rayo.d, baseLocalToGlobal, o, false);
    rayo.o = cambioBase(rayo.o, baseLocalToGlobal, o, false);
    rayo.d = normalizar(rayo.d);
}

bool Camara::iluminar(const Punto& p0, const Direccion& normal, const Escena& escena,
                        const float coefDifuso, RGB& radiancia) const {
    bool iluminar = escena.puntoIluminado(p0);
    if (!iluminar) return false;
    
    RGB radFinal = RGB({0.0f, 0.0f, 0.0f});
    for (LuzPuntual luz : escena.luces) {
        Direccion CMenosX = luz.c - p0;
        float termino3 = abs(dot(normal, CMenosX / modulo(CMenosX)));
        float termino2 = coefDifuso / PI;     // M_PI que se encuentra en <cmath>
        Direccion termino1 = luz.p / (modulo(CMenosX) * modulo(CMenosX));
        termino1 = termino1 * (termino2 * termino3);
        for (int i = 0; i < 3; ++i) {
            radFinal.rgb[i] += termino1.coord[i];
        }
    }
    radiancia = radFinal;
    return true;
}

void Camara::renderizarEscenaCentroPixel(unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, const std::string& nombreEscena,
                              float anchoPorPixel, float altoPorPixel, const float kd,
                              std::vector<std::vector<RGB>>& coloresEscena) const {

    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB emision;
            Punto ptoIntersec;
            Direccion normal;

            rayo = this->obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, this->o, this->f, this->u, this->l);
            if (escena.interseccion(rayo, emision, ptoIntersec, normal)) {
                RGB radiancia;
                if (!(this->iluminar(ptoIntersec, normal, escena, kd, radiancia))) {   // Si no hay luz directa allí
                    emision.rgb = {0.0f, 0.0f, 0.0f};     // Pintamos de negro
                } else {
                    emision = emision * radiancia;
                }
                coloresEscena[alto][ancho] = emision;
            }
        }
    }
}


void Camara::renderizarEscenaConAntialising(unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, const std::string& nombreEscena,
                              float anchoPorPixel, float altoPorPixel, const float kd,
                              std::vector<std::vector<RGB>>& coloresEscena, unsigned rpp) const {

    for (unsigned ancho = 0; ancho < numPxlsAncho; ancho++) {   
        for (unsigned alto = 0; alto < numPxlsAlto; alto++) {
            RGB emisionMedia;

            for(unsigned i = 0; i < rpp; i++){
                Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
                RGB emisionActual;
                Punto ptoIntersec;
                Direccion normal;

                rayo = this->obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, this->o, this->f, this->u, this->l);
                if (escena.interseccion(rayo, emisionActual, ptoIntersec, normal)) {
                    RGB radiancia;
                    if (this->iluminar(ptoIntersec, normal, escena, kd, radiancia)) {
                        emisionActual = emisionActual * radiancia;
                        emisionMedia = emisionMedia + emisionActual;
                    } // Si no se ilumina, no le sumamos nada (el rgb es 0,0,0)
                }
            }
            emisionMedia = emisionMedia / rpp;
            coloresEscena[alto][ancho] = emisionMedia;
        }
    }                  
}

void Camara::renderizarEscena(unsigned numPxlsAncho, unsigned numPxlsAlto, const Escena& escena,
                              const std::string& nombreEscena, unsigned rpp, const float kd) const {
    if (rpp < 1) {
        throw std::invalid_argument("Error: Division por cero no permitida.");
        return;
    }

    float anchoPorPixel = this->calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = this->calcularAltoPixel(numPxlsAlto);
   
    // Inicializado todo a color negro
    std::vector<std::vector<RGB>> coloresEscena(numPxlsAlto, std::vector<RGB>(numPxlsAncho,
                                                                              {0.0f, 0.0f, 0.0f}));
    if(rpp == 1){
        renderizarEscenaCentroPixel(numPxlsAncho, numPxlsAlto, escena, nombreEscena, anchoPorPixel,
                                    altoPorPixel, kd, coloresEscena);
    } else {
        renderizarEscenaConAntialising(numPxlsAncho, numPxlsAlto, escena, nombreEscena, anchoPorPixel,
                                       altoPorPixel, kd, coloresEscena, rpp);
    }
    //imprimirImagen(coloresEscena);
    pintarEscenaEnPPM(nombreEscena, 255.0f, 1.0f, coloresEscena);
}
