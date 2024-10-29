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
// #include <cmath>     // permite usar número pi


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

/*
// Devuelve False si y solo si no hay ninguna fuente de luz que incide sobre el punto p0
bool Camara::radiancia(const Punto& p0, const Escena& escena, const float coefDifuso, RGB* radiancia) {
    bool iluminar = escena.puntoIluminado(p0);
    if (!iluminar) return false;
    
    RGB radFinal = RGB({0.0f, 0.0f, 0.0f});
    for (LuzPuntual& luz : this->luces) {
        Distancia CMenosX = luz.c - p0;
        Distancia normal = ...   ;      // Habrá que sacarlo de alguna forma, igual desde escena.interseccion()
        float termino3 = modulo(dot(normal, CMenosX / modulo(CMenosX)));
        float termino2 = coefDifuso / M_PI;     // M_PI que se encuentra en <cmath>
        Direccion termino1 = luz.p / (modulo(CMenosX) * modulo(CMenosX));
        radFinal = radFinal + termino1 * (termino2 * termino3);
    }
    radiancia = radFinal;
    return true;
}
*/
/*
// Método que devuelve True si y solo si al punto <p0> lo ilumina una fuente de luz.
// En caso contrario, devuelve False.
bool Escena::puntoIluminado(const Punto& p0) {
    bool iluminar = true;
    for(LuzPuntual& luz : this->luces) {
        Direccion d = luz.centro - p0;
        Punto ptoMasCerca;
        RGB rgb;
        bool chocaObjeto = this->interseccion(Rayo(d, p0), rgb, ptoMasCerca);
        
        if (chocaObjeto) {
            iluminar = modulo(d) <= modulo(ptoMasCerca - p0);   // Si la fuente de luz está dentro de un objeto, también iluminamos
        }
        
        if (iluminar) break;
        }
    }
    return iluminar;
}
*/

void Camara::renderizarEscenaCentroPixel(unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, const std::string& nombreEscena,
                              float anchoPorPixel, float altoPorPixel, 
                              std::vector<std::vector<RGB>>& coloresEscena) const {

    for (unsigned ancho = 0; ancho < numPxlsAncho; ancho++) {   
        for (unsigned alto = 0; alto < numPxlsAlto; alto++) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB emision;
            Punto ptoIntersec;

            rayo = this->obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, this->o, this->f, this->u, this->l);
        
            if (escena.interseccion(rayo, emision, ptoIntersec)) {  // Si el rayo interseca con un objeto de la escena, se pinta
                // RGB radiancia;
                // iluminar = radiancia(ptoIntersec, escena, 0.5, radiancia);
                // if (!iluminar) {
                //     emision.rgb = {0.0f, 0.0f 0.0f};     Pintamos de negro
                // } else {
                //      emision = emision + radiancia;
                //      emision = toneMapping(emision);
                // }
                coloresEscena[alto][ancho] = emision;
            }
        }
    }
}


void Camara::renderizarEscenaConAntialising(unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, const std::string& nombreEscena,
                              float anchoPorPixel, float altoPorPixel, 
                              std::vector<std::vector<RGB>>& coloresEscena, unsigned rpp) const {

    for (unsigned ancho = 0; ancho < numPxlsAncho; ancho++) {   
        for (unsigned alto = 0; alto < numPxlsAlto; alto++) {
            RGB emisionMedia;

            for(unsigned i = 0; i < rpp; i++){
                Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
                RGB emisionActual;
                Punto ptoIntersec;

                rayo = this->obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, this->o, this->f, this->u, this->l);
            
                if (escena.interseccion(rayo, emisionActual, ptoIntersec)) {    // Si el rayo interseca con un objeto de la escena, se pinta
                    emisionMedia = emisionMedia + emisionActual;
                }
            }
            emisionMedia = emisionMedia / rpp;
            coloresEscena[alto][ancho] = emisionMedia;

        }
    }                  
}

void Camara::renderizarEscena(unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, const std::string& nombreEscena, unsigned rpp) const {
    // ESTO DEBERÍA SER LA EXCEPCIÓN ESA DE ARGUMENTO INVÁLIDO
    // ESTO DEBERÍA SER LA EXCEPCIÓN ESA DE ARGUMENTO INVÁLIDO
    // ESTO DEBERÍA SER LA EXCEPCIÓN ESA DE ARGUMENTO INVÁLIDO
    // ESTO DEBERÍA SER LA EXCEPCIÓN ESA DE ARGUMENTO INVÁLIDO
    if (rpp < 1) {
        std::cout << "ERROR: RPP tiene que ser mayor que 0" << endl;
        return;
    }

    float anchoPorPixel = this->calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = this->calcularAltoPixel(numPxlsAlto);
   
    // Inicializado todo a color negro
    std::vector<std::vector<RGB>> coloresEscena(numPxlsAlto, std::vector<RGB>(numPxlsAncho,
                                                                              {0.0f, 0.0f, 0.0f}));

    if(rpp == 1){
        renderizarEscenaCentroPixel(numPxlsAncho, numPxlsAlto, escena,
                                        nombreEscena, anchoPorPixel, altoPorPixel, coloresEscena);
    } else {
        renderizarEscenaConAntialising(numPxlsAncho, numPxlsAlto, escena,
                                        nombreEscena, anchoPorPixel, altoPorPixel,coloresEscena, rpp);
    }
    //imprimirImagen(coloresEscena);
    pintarEscenaEnPPM(nombreEscena, 255.0f, 1.0f, coloresEscena);
}
