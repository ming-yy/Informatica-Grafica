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
#include <stack>

//const double M_PI = 3.14159265358979323846;   // Por si no va cmath
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

Rayo generarCaminoAleatorio(const Punto& o, const Direccion& normal) {
    float inclinacion, azimut;
    float x, y, z;

    generarAzimutInclinacion(azimut, inclinacion);
    getCoordenadasCartesianas(azimut, inclinacion, x, y, z);
    Direccion wi_local = normalizar(Direccion(x, y, z));      // Inclinacion positiva, hemisferio norte asegurado
    Direccion tangente;
    Direccion bitangente;
    construirBaseOrtonormal(normal, tangente, bitangente);
    
    // Cambio de base manual de <wi_local> de coord. locales a coord. globales
    Direccion nuevaDir = normalizar(tangente * wi_local.coord[0] +
                                    bitangente * wi_local.coord[1] +
                                    normal * wi_local.coord[2]);
    return Rayo(nuevaDir, o);
}

float calcBrdfDifusa(const float kd){
    return kd / M_PI;
}

float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n){
    return abs(dot(n, d / modulo(d)));
}

bool nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                         const float kd, RGB& radiancia, bool debug) {
    int n = 0;
    
    RGB radFinal = RGB({0.0f, 0.0f, 0.0f});
    for (LuzPuntual luz : escena.luces) {
        
        if(debug){
            n++;
            cout << endl << "(( LUZ " << n << " )) " << endl;
        }
        
        
        if (!escena.luzIluminaPunto(p0, luz)) {
            if(debug){
                cout << "(( NO iluminado )) " << endl << endl;
            }
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }
        
        
        Direccion CMenosX = luz.c - p0;
        float termino3 = calcCosenoAnguloIncidencia(CMenosX, normal);
        float termino2 = calcBrdfDifusa(kd);
        Direccion termino1 = luz.p / (modulo(CMenosX) * modulo(CMenosX));
        
        
        if(debug){
            cout << "(( luz.c: " << luz.c << " )) " << endl;
            cout << "(( p0: " << p0 << " )) " << endl;
            cout << "(( kd " << kd << " ))";
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
    
    for (const Primitiva* objeto : escena.primitivas) {
        Punto ptoLuz;
        Direccion luzPower(1,1,1);
        if (!(objeto->soyFuenteDeLuz()) || !escena.luzIluminaPunto(p0, objeto, ptoLuz)) {
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }
        Direccion CMenosX = ptoLuz - p0;
        float termino3 = calcCosenoAnguloIncidencia(CMenosX, normal);
        float termino2 = calcBrdfDifusa(kd);
        Direccion termino1 = luzPower / (modulo(CMenosX) * modulo(CMenosX));
        termino1 = termino1 * (termino2 * termino3);
        
        for (int i = 0; i < 3; ++i) {
            radFinal.rgb[i] += termino1.coord[i];
        }
    }
    
    radiancia = radFinal;
    
    return true;
}

void luzIndirectaIterativa(const Punto& origenInicial, const Direccion& normalInicial,
                           const Escena& escena, const float kd, const unsigned maxRebotes,
                           RGB& emisionAcumulada, float& brdfCosenoAcumulado, bool debug) {
    struct EstadoRebote {       // Estructura para almacenar el estado de cada rebote
        Punto origen;
        Direccion normal;
        unsigned rebotesRestantes;
        RGB emisionActual;
        float brdfCosenoActual;
    };

    std::stack<EstadoRebote> pila;      // Inicializar la pila con el primer estado
    pila.push({origenInicial, normalInicial, maxRebotes, RGB(0, 0, 0), 1.0f});

    while (!pila.empty()) {
        EstadoRebote estado = pila.top();
        pila.pop();

        if (debug) {
            cout << "==============================" << endl;
            cout << "REBOTES RESTANTES: " << estado.rebotesRestantes << endl;
            cout << "Emision acumulada antes: " << emisionAcumulada << endl;
        }

        // Condición terminal: no quedan rebotes
        if (estado.rebotesRestantes == 0) continue;

        Punto ptoIntersec;
        Direccion new_normal;
        RGB emisionActual;
        Rayo wi = generarCaminoAleatorio(estado.origen, estado.normal);

        bool choqueConLuz = false;
        bool hayIntersec = escena.interseccion(wi, emisionActual, ptoIntersec, new_normal, choqueConLuz);

        // Condición terminal: no hay intersección o el rayo choca con una fuente de luz
        if (!hayIntersec || (hayIntersec && choqueConLuz)) {
            if (debug) {
                cout << "Condición terminal, parando" << endl;
            }
            continue;
        }

        // Calcular radiancia y actualizar acumuladores
        RGB radianciaActual;
        nextEventEstimation(ptoIntersec, new_normal, escena, kd, radianciaActual, debug);
        
        if (debug) {
            cout << "RADIANCIA ACTUAL = " << radianciaActual << endl;
        }

        float nuevoBrdfCoseno = estado.brdfCosenoActual * calcBrdfDifusa(kd) *
                                calcCosenoAnguloIncidencia(estado.origen - ptoIntersec, estado.normal);
        emisionAcumulada = emisionAcumulada + emisionActual * radianciaActual * nuevoBrdfCoseno;

        if (debug) {
            cout << "Emision acumulada despues: " << emisionAcumulada << endl;
            cout << "BrdfCoseno acumulado despues: " << nuevoBrdfCoseno << endl;
            cout << "Punto intersección: " << ptoIntersec << endl;
            cout << "Nueva normal: " << new_normal << endl;
            cout << "==============================" << endl << endl;
        }

        // Apilar el siguiente estado de rebote si hay más rebotes restantes
        if (estado.rebotesRestantes > 1) {
            pila.push({ptoIntersec, new_normal, estado.rebotesRestantes - 1, emisionActual, nuevoBrdfCoseno});
        }
    }
}

void recursividadLuzIndirecta(const Punto& origen, const Direccion& normal,
                             const Escena& escena, const float kd, const unsigned rebotesRestantes,
                             RGB& emisionAcumulada, float& brdfCosenoAcumulado, bool debug) {
    Punto ptoIntersec;
    Direccion new_normal;
    RGB emisionActual;

    if (debug) {
        cout << "==============================" << endl;
        cout << "REBOTES RESTANTES: " << rebotesRestantes << endl;
        cout << "Emision acumulada antes: " << emisionAcumulada << endl;
    }
    
    if (rebotesRestantes == 0) return;    // Condición terminal: alcanzado max rebotes
    
    Rayo wi = generarCaminoAleatorio(origen, normal);
    bool choqueConLuz = false;
    bool hayIntersec = escena.interseccion(wi, emisionActual, ptoIntersec, new_normal, choqueConLuz);
    
    if (!hayIntersec) {  // Condición terminal: rayo no choca contra nada
        if (debug) {
            cout << "CONDICION TERMINAL: rayo no choca con nada" << endl;
        }
        return;
    } else if (hayIntersec && choqueConLuz) {   // Condición terminal: rayo choca contra fuente de luz de área
        if (debug) {
            cout << "CONDICION TERMINAL: rayo choca con luz" << endl;
        }
        
        RGB radianciaActual(1.0f, 1.0f, 1.0f);      // NextStepEstimation() ???
        brdfCosenoAcumulado = brdfCosenoAcumulado * calcBrdfDifusa(kd) *
                              calcCosenoAnguloIncidencia(origen - ptoIntersec, normal);
        emisionAcumulada = emisionAcumulada + emisionActual * radianciaActual * brdfCosenoAcumulado;
        return;
    } else {
        RGB radianciaActual;
        nextEventEstimation(ptoIntersec, new_normal, escena, kd, radianciaActual, debug);
        
        if (debug) {
            cout << "RADIANCIA ACTUAL = " << radianciaActual << endl;
        }
        
        brdfCosenoAcumulado = brdfCosenoAcumulado * calcBrdfDifusa(kd) *
                              calcCosenoAnguloIncidencia(origen - ptoIntersec, normal);
        emisionAcumulada = emisionAcumulada + emisionActual * radianciaActual * brdfCosenoAcumulado;
    }
    
    if (debug) {
        cout << "Emision acumulada despues: " << emisionAcumulada << endl;
        cout << "BrdfCoseno acumulado despues: " << brdfCosenoAcumulado << endl;
        cout << "Punto interseccion: " << ptoIntersec << endl;
        cout << "Nueva normal: " << new_normal << endl;
        cout << "==============================" << endl << endl;
    }
    
    recursividadLuzIndirecta(ptoIntersec, new_normal, escena, kd, rebotesRestantes - 1,
                             emisionAcumulada, brdfCosenoAcumulado, debug);
}

RGB obtenerEmisionIndirecta(const Escena& escena, const float kd, const unsigned maxRebotes,
                            const unsigned numRayosMontecarlo, const Punto& ptoIntersec,
                            const Direccion& normal, bool debug) {
    RGB emisionIndirecta;
    if (maxRebotes > 0) {
        for (unsigned i = 0; i < numRayosMontecarlo; ++i){   // Emisión media de N rayos por Montecarlo
            RGB emisionAcumulada;
            float brdfCosenoAcumulado = 1;
            if(debug){
                cout << endl << endl << endl << "    --- RAYO " << i+1 << endl << endl;
            }
            recursividadLuzIndirecta(ptoIntersec, normal, escena, kd, maxRebotes, emisionAcumulada,
                                     brdfCosenoAcumulado, debug);
            emisionIndirecta = emisionIndirecta + emisionAcumulada;
        }
    }
    emisionIndirecta = emisionIndirecta / numRayosMontecarlo;
    return emisionIndirecta;
}


void renderizarEscena1RPP(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const float kd, const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          std::vector<std::vector<RGB>>& coloresEscena) {
    
    
    // unsigned totalPixeles = numPxlsAlto * numPxlsAncho;
    // cout << "Procesando pixeles..." << endl;
    // cout << "0 pixeles de " << totalPixeles << endl;

    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB emisionDirecta;
            Punto ptoIntersec;
            Direccion normal;
            bool choqueConLuz = false;
            
            
            bool debug = false;
            //unsigned pixelActual = numPxlsAncho * ancho + alto + 1;
            //if (pixelActual % 100 == 0 || pixelActual == totalPixeles) {
            //    cout << pixelActual << " pixeles de " << totalPixeles << endl;
            //}
            
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            if (escena.interseccion(rayo, emisionDirecta, ptoIntersec, normal, choqueConLuz)) {
                RGB radianciaDirecta;
                nextEventEstimation(ptoIntersec, normal, escena, kd, radianciaDirecta, debug);
                emisionDirecta = emisionDirecta * radianciaDirecta;
                
                if (choqueConLuz) {
                    coloresEscena[alto][ancho] = emisionDirecta;
                } else {
                    RGB emisionIndirecta = obtenerEmisionIndirecta(escena, kd, maxRebotes, numRayosMontecarlo,
                                                                   ptoIntersec, normal, debug);
                    coloresEscena[alto][ancho] = emisionDirecta + emisionIndirecta;
                }
            }
        }
    }
}


/* Esto es lo que había para luz directa
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
                    } // Si no se ilumina, no le sumamos nada (el rgb default es {0,0,0})
                }
            }
            emisionMedia = emisionMedia / rpp;
            coloresEscena[alto][ancho] = emisionMedia;
        }
    }
}
*/


void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const std::string& nombreEscena, const unsigned rpp,
                      const float kd, const unsigned maxRebotes, const unsigned numRayosMontecarlo) {
    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
   
    // Inicializado todo a color negro
    std::vector<std::vector<RGB>> coloresEscena(numPxlsAlto, std::vector<RGB>(numPxlsAncho,
                                                                              {0.0f, 0.0f, 0.0f}));
    if(rpp == 1){
        renderizarEscena1RPP(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                             altoPorPixel, kd, maxRebotes, numRayosMontecarlo, coloresEscena);
    } else {
        //renderizarEscenaAntialising(camara, numPxlsAncho, numPxlsAlto,
        //                escena, anchoPorPixel, altoPorPixel, kd, coloresEscena, rpp);
    }
    
    //imprimirImagen(coloresEscena);
    pintarEscenaEnPPM(nombreEscena, 255.0f, 1.0f, coloresEscena);
}

