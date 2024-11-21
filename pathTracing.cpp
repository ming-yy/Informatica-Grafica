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
#include "utilidades.h"
#include <cmath>
#include <algorithm>
#include <random>
#include <stack>


void imprimirImagen(const vector<vector<RGB>>& imagen) {
    for (const auto& fila : imagen) {
        for (const auto& pixel : fila) {
            cout << "(" << pixel.rgb[0] << ", " << pixel.rgb[1] << ", " << pixel.rgb[2] << ") ";
        }
        cout << endl; // Nueva línea para cada fila
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
    Direccion wi_local = normalizar(Direccion(x, y, z));   // Inclinacion positiva, hemisferio norte asegurado
    Direccion tangente;
    Direccion bitangente;
    construirBaseOrtonormal(normal, tangente, bitangente);
    
    // Cambio de base manual de <wi_local> de coord. locales a coord. globales
    Direccion nuevaDir = normalizar(tangente * wi_local.coord[0] +
                                    bitangente * wi_local.coord[1] +
                                    normal * wi_local.coord[2]);
    return Rayo(nuevaDir, o);
}

Direccion calcDirEspecular(const Direccion& wo, const Direccion& n) {
    return normalizar(wo - n * 2.0f * dot(wo, n));
}

RGB calcBrdfDifusa(const RGB& kd){
    return kd / M_PI;
}

RGB calcBrdfEspecular(const RGB& ks) {
    return ks;
}

RGB calcBsdf(const BSDFs& coefs, int tipoRayo) {
    RGB resultado;
    
    switch (tipoRayo) {
        case 0:
            resultado = calcBrdfDifusa(coefs.kd);
            break;
        
        case 1:
            resultado = calcBrdfEspecular(coefs.ks);
            break;
        
        case 2:
            cout << "REFRACTANTE" << endl;
            break;
            
        case -1:
            resultado = RGB(0.0f, 0.0f, 0.0f);
            break;
            
        default:
            cerr << "Error: rayo no identificado" << endl;
            break;
    }
    
    return resultado;
}

// Falta hace un typedef o algo para que no devuelva un entero (?)
int dispararRuletaRusa(const BSDFs& coefs) {
    float maxKD = max(coefs.kd);
    float maxKS = max(coefs.ks);
    float maxKT = max(coefs.kt);
    //float total = maxKD + maxKS + maxKT;
    float total = 1.0f;
    
    if (total == 0) {   // No hay reflección ni refracción
        return -1;      // no corresponde con ningún rayo, error
    }

    float probDifuso = maxKD / total;
    float probEspecular = maxKS / total;
    float probRefractante = maxKT / total;
    float absorbente = 1 - probDifuso - probEspecular - probRefractante;
    
    //cout << "Probabilidades: " << probDifuso << " " << probEspecular << " " << probRefractante << " " << absorbente << endl;
    

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float bala = dist(gen);     // Random float entre (0,1)
    cout << bala << endl;

    if (bala <= probDifuso) {
        return 0;  // Rayo difuso
    } else if (bala <= probDifuso + probEspecular) {
        return 1;  // Rayo especular
    } else if (bala <= probDifuso + probEspecular + probRefractante) {
        return 2;  // Rayo refractante
    } else {
        //cout << "Absorbente" << endl;
        return -1;   // Absorbente
    }
}

Rayo obtenerTipoRayoRuletaRusa(const int tipoRayo, const Punto& origen, const Direccion& wo,
                                const Direccion& normal, bool debug) {
    Rayo wi;
    if (tipoRayo == 0) {        // difuso
        wi = generarCaminoAleatorio(origen, normal);
    } else if (tipoRayo == 1) {    // especular
        wi.d = calcDirEspecular(wo, normal);
        wi.o = origen;
    } else if (tipoRayo == 2){     // refractante
        cout << "Todavía no implementada" << endl;
    }
    return wi;
}

float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n){
    return abs(dot(n, d / modulo(d)));
}

RGB nextEventEstimation(const Direccion& wi, const Punto& p0, const Direccion& normal,
                        const Escena& escena, const RGB& kd, bool debug) {
    int n = 0;

    if (escena.puntoPerteneceALuz(p0)) {    // p0 es de una fuente de luz (de área)
        return RGB({1,1,1});    // Debería ser el power de la fuente de luz
    }
    
    RGB radianciaSaliente(0.0f, 0.0f, 0.0f);
    for (LuzPuntual luz : escena.luces) {
        
        if(debug){
            n++;
            cout << endl << "(( kd: " << kd << endl;
            cout << endl << "(( LUZ " << n << " )) " << endl;
        }
        
        
        if (!escena.luzIluminaPunto(p0, luz)) {
            if(debug){
                cout << "(( NO iluminado )) " << endl << endl;
            }
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }
        
        Direccion wr(0.0f, 0.0f, 0.0f);       // Por si tenemos un rayo especular
        Direccion dirIncidente = luz.c - p0;
        float cosAnguloIncidencia = calcCosenoAnguloIncidencia(dirIncidente, normal);
        RGB reflectanciaBrdfDifusa = calcBrdfDifusa(kd);
        RGB radianciaIncidente = luz.p / (modulo(dirIncidente) * modulo(dirIncidente));
        
        if(debug){
            cout << "(( luz.c: " << luz.c << " )) " << endl;
            cout << "(( luz.p: " << luz.p << " )) " << endl;
            cout << "(( p0: " << p0 << " )) " << endl;
            cout << "(( kd: " << kd << " ))";
            cout << "(( CmenosX: " << dirIncidente << " )) " << endl;
            cout << "(( radianciaIncidente: " << radianciaIncidente << " )) " << endl;
            cout << "(( reflectanciaBRDFDifusa: " << reflectanciaBrdfDifusa << " )) " << endl;
            cout << "(( cosAnguloIncidencia: " << cosAnguloIncidencia << " )) " << endl;
        }
        
        radianciaIncidente = radianciaIncidente * (reflectanciaBrdfDifusa * cosAnguloIncidencia);
        
        
        if(debug){
            cout << "(( iluminado: " << radianciaIncidente << " )) " << endl << endl;
        }
        
        
        for (int i = 0; i < 3; ++i) {
            radianciaSaliente.rgb[i] += radianciaIncidente.rgb[i];
        }
    }
    
    /*  Para luz de área
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

RGB recursividadRadianciaIndirecta(const Punto& origen, const Direccion &wo, const BSDFs &coefsOrigen, 
                                    const Direccion& normal, const Escena& escena,
                                    const unsigned rebotesRestantes, bool debug) {
    if (rebotesRestantes == 0) {     // Condición terminal: alcanzado max rebotes
        if (debug) cout << "REBOTES RESTANTES: 0" << endl;
        return RGB({0.0f, 0.0f, 0.0f});
    }
    
    int tipoRayo = dispararRuletaRusa(coefsOrigen);
    if (tipoRayo != -1) {
        return RGB({0.0f, 0.0f, 0.0f});
    }
    Rayo wi = obtenerTipoRayoRuletaRusa(tipoRayo, origen, wo, normal, debug);

    bool choqueConLuz = false;
    BSDFs coefsPtoIntersec;
    Punto ptoIntersec;
    Direccion nuevaNormal;
    bool hayIntersec = escena.interseccion(wi, coefsPtoIntersec, ptoIntersec, nuevaNormal, choqueConLuz);
    
     if (debug) {
        cout << "==============================" << endl;
        cout << "REBOTES RESTANTES: " << rebotesRestantes << endl;
        cout << "Punto interseccion: " << ptoIntersec << endl;
        cout << "Nueva normal: " << nuevaNormal << endl;
        cout << "coefsPtoInterseccion: " << coefsPtoIntersec << endl;
        cout << "==============================" << endl << endl;
    }

    if (!hayIntersec) {    // Condición terminal: rayo no choca contra nada, devuelve (0,0,0)
        if (debug) cout << "CONDICION TERMINAL: rayo no choca con nada" << endl;
        return RGB({0.0f, 0.0f, 0.0f});
    } else if (hayIntersec && choqueConLuz) {   // Condición terminal: rayo choca contra fuente de luz de área
                                                //                      devuelve emisión de la luz
        if (debug) cout << "CONDICION TERMINAL: rayo choca con luz" << endl << "coefs: " << coefsPtoIntersec << endl;
        return coefsPtoIntersec.kd;    // RECORDAR: terminar de implementar luz de área
                                   // debería devolver un RGB(1,1,1) o el power que tenga
    }
    
    RGB radianciaSalienteDirecta(0.0f, 0.0f, 0.0f);
    if (tipoRayo == 1) {    // Si es difuso
        radianciaSalienteDirecta = nextEventEstimation(wi.d, ptoIntersec, nuevaNormal, escena, 
                                                        coefsPtoIntersec.kd, debug);
    }
    
    RGB radianciaSalienteIndirecta = recursividadRadianciaIndirecta(ptoIntersec, wi.d, coefsPtoIntersec, nuevaNormal,
                                                                    escena, rebotesRestantes - 1, debug)
                                    * calcBsdf(coefsPtoIntersec, tipoRayo)
                                    * calcCosenoAnguloIncidencia(origen - ptoIntersec, normal);
    
    if (debug) cout << "LUZ INTERSECCION = " << radianciaSalienteDirecta << endl;

    return radianciaSalienteDirecta + radianciaSalienteIndirecta;
           
}

RGB obtenerRadianciaSalienteIndirecta(const Escena& escena, const unsigned maxRebotes, 
                                        const unsigned numRayosMontecarlo, const Punto& ptoIntersec,
                                        const Direccion& wo, const BSDFs &coefsPtoInterseccion,
                                        const Direccion& normal, bool debug) {
    RGB sumaRadianciasIndirectas;
    if (maxRebotes > 0) {
        for (unsigned i = 0; i < numRayosMontecarlo; ++i){   // Emisión media de N rayos por Montecarlo
            if(debug) cout << endl << endl << endl << "    --- RAYO " << i+1 << endl << endl;
            
            sumaRadianciasIndirectas = sumaRadianciasIndirectas +
                                        recursividadRadianciaIndirecta(ptoIntersec, wo, coefsPtoInterseccion, 
                                                                        normal, escena, maxRebotes, debug);
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

RGB obtenerRadianciaSaliente(Rayo &rayoIncidente, const Escena &escena, const unsigned maxRebotes, 
                             const unsigned numRayosMontecarlo, bool debug){
    Punto ptoIntersec;
    Direccion normal;
    bool choqueConLuz = false;
    RGB radianciaSalienteTotal;
    BSDFs coefsPtoInterseccion;
    
    if (escena.interseccion(rayoIncidente, coefsPtoInterseccion, ptoIntersec, normal, choqueConLuz)) {
        RGB radianciaSalienteDirecta(0.0f, 0.0f, 0.0f);
        int tipoRayo = dispararRuletaRusa(coefsPtoInterseccion);
        if (tipoRayo == -1) {
            return RGB({0.0f, 0.0f, 0.0f});
        } else if (tipoRayo == 1) {
            radianciaSalienteDirecta = nextEventEstimation(rayoIncidente.d, ptoIntersec, normal, escena,
                                                           coefsPtoInterseccion.kd , debug);
        }

        radianciaSalienteTotal = radianciaSalienteDirecta;
        if (!choqueConLuz) {
            radianciaSalienteTotal = radianciaSalienteTotal +
                                     obtenerRadianciaSalienteIndirecta(escena, maxRebotes, numRayosMontecarlo,
                                                                       ptoIntersec, rayoIncidente.d, coefsPtoInterseccion,
                                                                       normal, debug);
        }
    }
    return radianciaSalienteTotal;
}

void renderizarEscena1RPP(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const unsigned totalPixeles,
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
                          vector<vector<RGB>>& coloresEscena, const bool printPixelesProcesados,
                          const unsigned totalPixeles, const unsigned rpp) {

    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());            
            
            if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);

            bool debug = false;
            RGB radianciaSalienteTotal;
            for(unsigned i = 0; i < rpp; ++i){
                rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                radianciaSalienteTotal = radianciaSalienteTotal +
                                         obtenerRadianciaSaliente(rayo, escena, maxRebotes, numRayosMontecarlo, debug);
            }
            coloresEscena[alto][ancho] = radianciaSalienteTotal / rpp;
        }
    }
}


void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const string& nombreEscena, const unsigned rpp,
                      const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                      const bool printPixelesProcesados) {
    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
   
    unsigned totalPixeles = numPxlsAlto * numPxlsAncho;
    if (printPixelesProcesados) cout << "Procesando pixeles..." << endl << "0 pixeles de " << totalPixeles << endl;

    // Inicializado todo a color negro
    vector<vector<RGB>> coloresEscena(numPxlsAlto, vector<RGB>(numPxlsAncho, {0.0f, 0.0f, 0.0f}));

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

