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

RGB calcBrdfDifusa(const RGB &kd){
    return kd / M_PI;
}

float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n){
    return abs(dot(n, d / modulo(d)));
}

RGB nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                         const RGB &kd, bool debug) {
    int n = 0;

    if (escena.puntoPerteneceALuz(p0)) {    // p0 es de una fuente de luz
        return RGB({1,1,1});    // Debería ser el power de la fuente de luz
    }
    
    RGB radianciaSaliente;
    for (LuzPuntual luz : escena.luces) {
        
        if(debug){
            n++;
            cout << endl << "(( kd antes: " << kd << endl;
            cout << endl << "(( LUZ " << n << " )) " << endl;
        }
        
        
        if (!escena.luzIluminaPunto(p0, luz)) {
            if(debug){
                cout << "(( NO iluminado )) " << endl << endl;
            }
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }
        
        Direccion dirIncidente = luz.c - p0;
        float cosAnguloIncidencia = calcCosenoAnguloIncidencia(dirIncidente, normal);
        RGB reflectanciaBRDFDifusa = calcBrdfDifusa(kd);
        RGB radianciaIncidente = luz.p / (modulo(dirIncidente) * modulo(dirIncidente));
        
        if(debug){
            cout << "(( luz.c: " << luz.c << " )) " << endl;
            cout << "(( luz.p: " << luz.p << " )) " << endl;
            cout << "(( p0: " << p0 << " )) " << endl;
            cout << "(( kd " << kd << " ))";
            cout << "(( CmenosX: " << dirIncidente << " )) " << endl;
            cout << "(( radianciaIncidente: " << radianciaIncidente << " )) " << endl;
            cout << "(( reflectanciaBRDFDifusa: " << reflectanciaBRDFDifusa << " )) " << endl;
            cout << "(( cosAnguloIncidencia: " << cosAnguloIncidencia << " )) " << endl;
        }
        
        radianciaIncidente = radianciaIncidente * (reflectanciaBRDFDifusa * cosAnguloIncidencia);
        
        
        if(debug){
            cout << "(( iluminado: " << radianciaIncidente << " )) " << endl << endl;
        }
        
        
        for (int i = 0; i < 3; ++i) {
            radianciaSaliente.rgb[i] += radianciaIncidente.rgb[i];
        }
    }
    
    /*
    for (const Primitiva* objeto : escena.primitivas) {
        Punto origenLuz;
        Direccion luzPower(1,1,1);
        if (!(objeto->soyFuenteDeLuz()) || !escena.luzIluminaPunto(p0, objeto, origenLuz)) {
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }
        Direccion dirIncidente = origenLuz - p0;
        float cosAnguloIncidencia = calcCosenoAnguloIncidencia(dirIncidente, normal);
        float reflectanciaBRDFDifusa = calcBrdfDifusa(kd);
        Direccion radianciaIncidente = luzPower / (modulo(dirIncidente) * modulo(dirIncidente));
        radianciaIncidente = radianciaIncidente * (reflectanciaBRDFDifusa * cosAnguloIncidencia);
        
        for (int i = 0; i < 3; ++i) {
            radianciaSaliente.rgb[i] += radianciaIncidente.coord[i];
        }
    }
    */
    
    return radianciaSaliente;
}

/*
void iterativaRadianciaIndirecta(const Punto& origenInicial, const Direccion& normalInicial,
                           const Escena& escena, const float kd, const unsigned maxRebotes,
                           RGB& radianciaAcumulada, float& brdfCosenoAcumulado, bool debug) {
    struct EstadoRebote {       // Estructura para almacenar el estado de cada rebote
        Punto origen;
        Direccion normal;
        unsigned rebotesRestantes;
        RGB radianciaActual;
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
            cout << "Radiancia acumulada antes: " << radianciaAcumulada << endl;
        }

        // Condición terminal: no quedan rebotes
        if (estado.rebotesRestantes == 0) continue;

        Punto ptoIntersec;
        Direccion nuevaNormal;
        BSDFs coeficientes;
        Rayo wi = generarCaminoAleatorio(estado.origen, estado.normal);

        bool choqueConLuz = false;
        bool hayIntersec = escena.interseccion(wi, coeficientes, ptoIntersec, nuevaNormal, choqueConLuz);

        // Condición terminal: no hay intersección o el rayo choca con una fuente de luz
        if (!hayIntersec || (hayIntersec && choqueConLuz)) {
            if (debug) {
                cout << "Condición terminal, parando" << endl;
            }
            continue;
        }

        RGB radianciaActual = nextEventEstimation(ptoIntersec, nuevaNormal, escena, kd, debug);
        
        if (debug) {
            cout << "RADIANCIA ACTUAL = " << radianciaActual << endl;
        }

        float nuevoBrdfCoseno = estado.brdfCosenoActual * calcBrdfDifusa(kd) *
                                calcCosenoAnguloIncidencia(estado.origen - ptoIntersec, estado.normal);
        radianciaAcumulada = radianciaAcumulada + coeficientes.kd * radianciaActual * nuevoBrdfCoseno;

        if (debug) {
            cout << "Emision acumulada despues: " << radianciaAcumulada << endl;
            cout << "BrdfCoseno acumulado despues: " << nuevoBrdfCoseno << endl;
            cout << "Punto intersección: " << ptoIntersec << endl;
            cout << "Nueva normal: " << nuevaNormal << endl;
            cout << "==============================" << endl << endl;
        }

        // Apilar el siguiente estado de rebote si hay más rebotes restantes
        if (estado.rebotesRestantes > 1) {
            pila.push({ptoIntersec, nuevaNormal, estado.rebotesRestantes - 1, radianciaActual, nuevoBrdfCoseno});
        }
    }
}
*/

RGB recursividadRadianciaIndirecta(const Punto& origen, const Direccion& normal,
                             const Escena& escena, const unsigned rebotesRestantes, bool debug) {
    Punto ptoIntersec;
    Direccion nuevaNormal;
    BSDFs coeficientes;

    if (rebotesRestantes == 0) cout << "REBOTES RESTANTES: 0" << endl; return RGB();    // Condición terminal: alcanzado max rebotes,
                                                //                      devuelve (0,0,0)
    Rayo wi = generarCaminoAleatorio(origen, normal);
    bool choqueConLuz = false;
    bool hayIntersec = escena.interseccion(wi, coeficientes, ptoIntersec, nuevaNormal, choqueConLuz);
    
     if (debug) {
        cout << "==============================" << endl;
        cout << "REBOTES RESTANTES: " << rebotesRestantes << endl;
        cout << "Punto interseccion: " << ptoIntersec << endl;
        cout << "Nueva normal: " << nuevaNormal << endl;
        cout << "coeficientes: " << coeficientes << endl;
        cout << "==============================" << endl << endl;
    }

    if (!hayIntersec) {    // Condición terminal: rayo no choca contra nada, devuelve (0,0,0)
        if (debug) cout << "CONDICION TERMINAL: rayo no choca con nada" << endl;
        return RGB();
    } else if (hayIntersec && choqueConLuz) {   // Condición terminal: rayo choca contra fuente de luz de área
                                                //                      devuelve emisión de la luz
        if (debug) cout << "CONDICION TERMINAL: rayo choca con luz" << endl << "coefs: " << coeficientes << endl;
        return coeficientes.kd;    // RECORDAR: terminar de implementar luz de área
    }

    
    // Calcular mediante RULETA RUSA si el rayo es difuso, reflectante, refractante o si se absorbe
    RGB radianciaSalienteDirecta = coeficientes.kd * nextEventEstimation(ptoIntersec, nuevaNormal, escena, coeficientes.kd, debug);
    RGB radianciaSalienteIndirecta = recursividadRadianciaIndirecta(ptoIntersec, nuevaNormal, escena,
                                                                rebotesRestantes - 1, debug)
                                    * calcBrdfDifusa(coeficientes.kd)
                                    * calcCosenoAnguloIncidencia(origen - ptoIntersec, normal);
    
    if (debug) cout << "LUZ INTERSECCION = " << radianciaSalienteDirecta << endl;

    return radianciaSalienteDirecta + radianciaSalienteIndirecta;
           
}

RGB obtenerRadianciaSalienteIndirecta(const Escena& escena, const unsigned maxRebotes,
                            const unsigned numRayosMontecarlo, const Punto& ptoIntersec,
                            const Direccion& normal, bool debug) {
    RGB sumaRadianciasIndirectas;
    if (maxRebotes > 0) {
        for (unsigned i = 0; i < numRayosMontecarlo; ++i){   // Emisión media de N rayos por Montecarlo
            if(debug) cout << endl << endl << endl << "    --- RAYO " << i+1 << endl << endl;
            
            sumaRadianciasIndirectas = sumaRadianciasIndirectas +
                                        recursividadRadianciaIndirecta(ptoIntersec, normal, escena, 
                                                                            maxRebotes, debug);
        }
    }
    return sumaRadianciasIndirectas / numRayosMontecarlo;       // Media de todas
}


void printPixelActual(unsigned totalPixeles, unsigned numPxlsAncho, unsigned ancho, unsigned alto){
    unsigned pixelActual = numPxlsAncho * ancho + alto + 1;
    if (pixelActual % 100 == 0 || pixelActual == totalPixeles) {
        cout << pixelActual << " pixeles de " << totalPixeles << endl;
    }
}

RGB obtenerRadianciaSaliente(Rayo &rayo, const Escena &escena, const unsigned maxRebotes, 
                                const unsigned numRayosMontecarlo, bool debug){
    BSDFs coefsDirectos;
    Punto ptoIntersec;
    Direccion normal;
    bool choqueConLuz = false;
    RGB radianciaSaliente;

    if (escena.interseccion(rayo, coefsDirectos, ptoIntersec, normal, choqueConLuz)) {
        // Calcular mediante RULETA RUSA si el rayo es difuso, reflectante, refractante o si se absorbe
        RGB radianciaDirecta = nextEventEstimation(ptoIntersec, normal, escena, coefsDirectos.kd, debug);
        RGB radianciaSalienteDirecta = coefsDirectos.kd * radianciaDirecta;
        
        if (choqueConLuz) {
            radianciaSaliente = radianciaSalienteDirecta;
        } else {
            radianciaSaliente = radianciaSalienteDirecta + obtenerRadianciaSalienteIndirecta(escena, maxRebotes, numRayosMontecarlo,
                                                                                            ptoIntersec, normal, debug);
        }
    }

    return radianciaSaliente;
}

void renderizarEscena1RPP(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          std::vector<std::vector<RGB>>& coloresEscena, const unsigned totalPixeles, 
                          const bool printPixelesProcesados) {
    
    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            
            if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);
                
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            
            bool debug = false;
            coloresEscena[alto][ancho] = obtenerRadianciaSaliente(rayo, escena, maxRebotes, numRayosMontecarlo, debug);
        }
    }
}

void renderizarEscenaConAntialiasing(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          std::vector<std::vector<RGB>>& coloresEscena, const bool printPixelesProcesados, 
                          const unsigned totalPixeles, const unsigned rpp) {

    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());            
            
            if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);

            bool debug = false;
            RGB radianciaSalienteTotal;
            for(unsigned i = 0; i < rpp; i++){
                rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                
                radianciaSalienteTotal = radianciaSalienteTotal + obtenerRadianciaSaliente(rayo, escena, maxRebotes, numRayosMontecarlo, debug);
            }
            coloresEscena[alto][ancho] = radianciaSalienteTotal / rpp;
        }
    }
}


void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const std::string& nombreEscena, const unsigned rpp,
                      const unsigned maxRebotes, const unsigned numRayosMontecarlo, const bool printPixelesProcesados) {
    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
   
    unsigned totalPixeles = numPxlsAlto * numPxlsAncho;
    if (printPixelesProcesados) cout << "Procesando pixeles..." << endl << "0 pixeles de " << totalPixeles << endl;

    // Inicializado todo a color negro
    std::vector<std::vector<RGB>> coloresEscena(numPxlsAlto, std::vector<RGB>(numPxlsAncho,
                                                                              {0.0f, 0.0f, 0.0f}));

    if(rpp == 1){
        renderizarEscena1RPP(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                             altoPorPixel, maxRebotes, numRayosMontecarlo, coloresEscena,
                             totalPixeles, printPixelesProcesados);
    } else {
        renderizarEscenaConAntialiasing(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                                    altoPorPixel, maxRebotes, numRayosMontecarlo, coloresEscena,
                                    printPixelesProcesados, totalPixeles, rpp);
    }
    
    //imprimirImagen(coloresEscena);
    pintarEscenaEnPPM(nombreEscena, coloresEscena);
}

