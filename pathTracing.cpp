//*****************************************************************
// File:   pathTracing.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#include "pathTracing.h"
#include "rgb.h"
#include "base.h"
#include "transformaciones.h"
#include "gestorPPM.h"
#include <cmath>
#include <random>

const double M_PI = 3.14159265358979323846;   // Por si no va cmath
#define GRAD_A_RAD 3.1415926535898f/180

using std::cout;
using std::endl;


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

void construirBaseOrtonormal(const Direccion& normal, Direccion& tangente, Direccion& bitangente) {
    if (fabs(normal.coord[0]) > fabs(normal.coord[2])) {
        tangente = Direccion(-normal.coord[1], normal.coord[0], 0);
    } else {
        tangente = Direccion(0, -normal.coord[2], normal.coord[1]);
    }
    tangente = normalizar(tangente);
    bitangente = cross(normal, tangente);
}

void generarAzimutInclinacion(float& azimut, float& inclinacion) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    inclinacion = acos(sqrt(1-dis(gen)));
    azimut = 2 * M_PI * dis(gen);
}

void getCoordenadasCartesianas(const float azimut, const float inclinacion,
                                float& x, float& y, float& z) {
    float sinAzim = static_cast<float>(sin(float(azimut)));
    float sinIncl = static_cast<float>(sin(float(inclinacion)));
    float cosAzim = static_cast<float>(cos(float(azimut)));
    float cosIncl = static_cast<float>(cos(float(inclinacion)));
    x = sinIncl * cosAzim;
    y = sinIncl * sinAzim;
    z = cosIncl;
}

Rayo generarCaminoAleatorio(const Punto& o, const Direccion& normal, bool debug) {
    float inclinacion, azimut;
    float x, y, z;

    generarAzimutInclinacion(azimut, inclinacion);
    getCoordenadasCartesianas(azimut, inclinacion, x, y, z);
    Direccion wi_local = normalizar(Direccion(x, y, z));      // Inclinacion positiva, hemisferio norte asegurado
    if (debug) {
        cout << "Inclinacion: " << inclinacion << endl;
        cout << "Azimut: " << azimut << endl;
        cout << "XYZ: " << x << " " << y << " " << z << endl;
        cout << "Wi local: " << wi_local << endl;
    }

    Direccion tangente;
    Direccion bitangente;
    construirBaseOrtonormal(normal, tangente, bitangente);
    // Cambio de base manual de <wi_local> de coord. locales a coord. globales

    Direccion nuevaDir = normalizar(tangente * wi_local.coord[0] +
                                    bitangente * wi_local.coord[1] +
                                    normal * wi_local.coord[2]);

    if (debug) {
        cout << "Normal: " << normal << endl;
        cout << "Tangente: " << tangente << endl;
        cout << "Bitangente: " << bitangente << endl;
        cout << "Nueva direccion: " << nuevaDir << endl;
    }
    return Rayo(nuevaDir, o);
}

float evaluacionBRDFdifusa(const float coefDifuso){
    return coefDifuso / M_PI;
}

bool nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                         const float coefDifuso, RGB& radiancia, bool debug) {
    // SOLO TIENE SENTIDO EN LUZ DIRECTA
    //bool iluminar = escena.puntoIluminado(p0);
    //if (!iluminar) return false;
    int n = 0;
    RGB radFinal = RGB({0.0f, 0.0f, 0.0f});
    for (LuzPuntual luz : escena.luces) {
        n++;
        if(debug){
            cout << endl << "(( LUZ " << n << " )) " << endl;
        }
        if (!escena.luzIluminaPunto(p0, luz)) {
            if(debug){
                cout << "(( NO iluminado )) " << endl << endl;
            }
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }
        Direccion CMenosX = luz.c - p0;
        float termino3 = abs(dot(normal, CMenosX / modulo(CMenosX)));
        float termino2 = evaluacionBRDFdifusa(coefDifuso);
        Direccion termino1 = luz.p / (modulo(CMenosX) * modulo(CMenosX));
        if(debug){
            cout << "(( luz.c: " << luz.c << " )) " << endl;
            cout << "(( p0: " << p0 << " )) " << endl;
            cout << "(( coefDifuso " << coefDifuso << " ))";
            cout << "(( CmenosX: " << CMenosX << " )) " << endl;
            cout << "(( termino1: " << termino1 << " )) " << endl;
            cout << "(( termino2: " << termino2 << " )) " << endl;
            cout << "(( termino3: " << termino3 << " )) " << endl;
        }
        termino1 = termino1 * (termino2 * termino3);
        if(debug){
            cout << "(( iluminado: " << termino1 << " )) " << endl << endl;
        }
        for (int i = 0; i < 3; ++i) {
            radFinal.rgb[i] += termino1.coord[i];
        }
    }
    radiancia = radFinal;
    return true;
}

void renderizarEscena1RPP(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, float anchoPorPixel, float altoPorPixel,
                              const float kd, std::vector<std::vector<RGB>>& coloresEscena) {

    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB emision;
            Punto ptoIntersec;
            Direccion normal;

            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            if (escena.interseccion(rayo, emision, ptoIntersec, normal)) {
                RGB radiancia;
                if (!(nextEventEstimation(ptoIntersec, normal, escena, kd, radiancia, false))) {   // Si no hay luz directa allí
                    emision.rgb = {0.0f, 0.0f, 0.0f};     // Pintamos de negro
                } else {
                    emision = emision * radiancia;
                }
                coloresEscena[alto][ancho] = emision;
            }
        }
    }
}


void renderizarEscenaConAntialising(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                              const Escena& escena, float anchoPorPixel, float altoPorPixel, 
                              const float kd, std::vector<std::vector<RGB>>& coloresEscena,
                              const unsigned rpp) {
    for (unsigned ancho = 0; ancho < numPxlsAncho; ancho++) {
        for (unsigned alto = 0; alto < numPxlsAlto; alto++) {
            RGB emisionMedia;

            for(unsigned i = 0; i < rpp; i++){
                Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
                RGB emisionActual;
                Punto ptoIntersec;
                Direccion normal;

                rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                if (escena.interseccion(rayo, emisionActual, ptoIntersec, normal)) {
                    RGB radiancia;
                    if (nextEventEstimation(ptoIntersec, normal, escena, kd, radiancia, false)) {
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

void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto, const Escena& escena,
                              const std::string& nombreEscena, const unsigned rpp, const float kd) {
    if (rpp < 1) {
        throw std::invalid_argument("Error: Division por cero no permitida.");
        return;
    }

    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
   
    // Inicializado todo a color negro
    std::vector<std::vector<RGB>> coloresEscena(numPxlsAlto, std::vector<RGB>(numPxlsAncho,
                                                                              {0.0f, 0.0f, 0.0f}));
    if(rpp == 1){
        renderizarEscena1RPP(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                                    altoPorPixel, kd, coloresEscena);
    } else {
        renderizarEscenaConAntialising(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                                       altoPorPixel, kd, coloresEscena, rpp);
    }
    
    //imprimirImagen(coloresEscena);
    pintarEscenaEnPPM(nombreEscena, 255.0f, 1.0f, coloresEscena);
}


void recursividadLuzIndirecta(const Punto& origen, const Direccion& normal,
                             const Escena& escena, const float kd,
                             const unsigned rebotesRestantes, RGB& emisionAcumulada, bool debug) {
    Punto ptoIntersec;
    Direccion new_normal;
    bool choqueConLuz = false;
    RGB emisionActual;

    if (debug) {
        cout << "==============================" << endl;
        cout << "REBOTES RESTANTES: " << rebotesRestantes << endl;
        cout << "Emision acumulada antes: " << emisionAcumulada << endl;
    }
    
    if (rebotesRestantes == 0) return;    // Condición terminal: alcanzado max rebotes
    Rayo wi = generarCaminoAleatorio(origen, normal, debug);
    bool hayIntersec = escena.interseccion(wi, emisionActual, ptoIntersec, new_normal);
    
    // Condición terminal: 1) rayo no choca contra nada
    //                     2) rayo choca contra fuente de luz de área
    if (!hayIntersec || (hayIntersec && choqueConLuz)) {
        if (debug) {
            cout << "Condicion terminal, parando" << endl;
        }
        return;
    } else {
        // Calcula la emision del rebote
        RGB radianciaActual;
        nextEventEstimation(ptoIntersec, new_normal, escena, kd, radianciaActual, debug);
        if (debug) {
            cout << "RADIANCIA ACTUAL = " << radianciaActual << endl;
        }
        //brdf_coseno.push_back(radiancia);
        //for (auto termino: brdf_coseno) {
        //    emisionAcumulada = emisionAcumulada + emisionActual * termino;
        //}
        emisionAcumulada = emisionAcumulada + emisionActual * radianciaActual;
    }
    
    if (debug) {
        cout << "Emision acumulada despues: " << emisionAcumulada << endl;
        cout << "Punto interseccion: " << ptoIntersec << endl;
        cout << "Nueva normal: " << new_normal << endl;
        cout << "==============================" << endl << endl;
    }
    recursividadLuzIndirecta(ptoIntersec, new_normal, escena, kd, rebotesRestantes - 1, emisionAcumulada, debug);
}


void renderizarEscena1RPPLuzIndirecta(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                                      const Escena& escena, float anchoPorPixel, float altoPorPixel,
                                      const float kd, const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                                      std::vector<std::vector<RGB>>& coloresEscena) {
    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB emisionDirecta;
            Punto ptoIntersec;
            Direccion normal;

            

             
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            if (escena.interseccion(rayo, emisionDirecta, ptoIntersec, normal)) {
                RGB radiancia;
                //bool debug = (ancho == 20 && alto == 50);
                bool debug = false;
                nextEventEstimation(ptoIntersec, normal, escena, kd, radiancia, debug);   // Si no hay luz directa allí
                emisionDirecta = emisionDirecta * radiancia;

                // Calculamos la emisión media de N rayos de sampleo Montecarlo
                RGB emisionIndirecta;
                for(unsigned i = 0; i < numRayosMontecarlo; i++){
                    // Luz indirecta
                    //std::vector<RGB> brdf_coseno;
                    //brdf_coseno.push_back(radiancia);
                    RGB emisionAcumulada;
                    if(debug){
                        cout << endl << endl << endl << "    --- RAYO " << i+1 << endl << endl;
                    }
                    recursividadLuzIndirecta(ptoIntersec, normal, escena, kd, maxRebotes, emisionAcumulada, debug);
                    emisionIndirecta = emisionIndirecta + emisionAcumulada;
                }

                emisionIndirecta = emisionIndirecta / numRayosMontecarlo; // la media de todos
                coloresEscena[alto][ancho] = emisionDirecta + emisionIndirecta;
            }
        }
    }
}


void renderizarEscenaLuzIndirecta(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                                  const Escena& escena, const std::string& nombreEscena, const unsigned rpp,
                                  const float kd, const unsigned maxRebotes, const unsigned numRayosMontecarlo) {
    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
   
    // Inicializado todo a color negro
    std::vector<std::vector<RGB>> coloresEscena(numPxlsAlto, std::vector<RGB>(numPxlsAncho,
                                                                              {0.0f, 0.0f, 0.0f}));
    if(rpp == 1){
        renderizarEscena1RPPLuzIndirecta(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                                         altoPorPixel, kd, maxRebotes, numRayosMontecarlo, coloresEscena);
    } else {
        //renderizarEscenaAntialisingLuzIndirecta(camara, numPxlsAncho, numPxlsAlto,
        //                escena, anchoPorPixel, altoPorPixel, kd, coloresEscena, rpp);
    }
    
    //imprimirImagen(coloresEscena);
    pintarEscenaEnPPM(nombreEscena, 255.0f, 1.0f, coloresEscena);
}



/*
funcion renderizarEscenaLuzIndirecta (...) {
    ...
    emision, origen, normal = renderizarEscenaAntialiasing / renderizarEscenaCentroPixel (...)
    // emision = ... lo que sea, asumimos que con esto obtenemos la emisión L0
    // normal = ... la que sea que hayamos obtenido
    // origen = el punto que sea que hayamos obtenido
    recursividadLuzIndirecta(emision, origen, normal)
}
*/
 

