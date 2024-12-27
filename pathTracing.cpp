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
#include <thread>
#include <atomic>

std::atomic<unsigned> pixelesProcesados{0};


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

Direccion generarDireccionAleatoria(const Direccion& normal, float& prob) {
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
    
    // Probabilidad de un rayo es proporcional al coseno del ángulo de inclinación
    prob = cos(inclinacion) / M_PI;
    return nuevaDir;
}

Direccion calcDirEspecular(const Direccion& wo, const Direccion& n) {
    return normalizar(wo - n * 2.0f * dot(wo, n));
}

std::optional<Direccion> calcDirRefractante(const Direccion& wo, const Direccion& normal,
                                            const float ni, const float nr) {
    Direccion woo = wo * (-1);      // woo apunta hacia la cámara
    Direccion n = normal;
    float eta = nr / ni;
    float cosThetaI = dot(n, woo);
    if (cosThetaI < 0) {    // Rayo entrando al material
        eta = 1 / eta;
        cosThetaI = - cosThetaI;
        n = n * (-1);
    }
    float sin2ThetaI = max(0.0f, 1 - cosThetaI * cosThetaI);
    float sin2ThetaT = sin2ThetaI / (eta * eta);
    if (sin2ThetaT >= 1) {      // Reflexión interna total
        return std::nullopt;
    }
    
    float cosThetaT = sqrt(max(0.0f, 1.0f - sin2ThetaT));
    Direccion wt = (woo * (-1)) / eta + n * (cosThetaI / eta - cosThetaT);
    return normalizar(wt);
    
}

Rayo obtenerRayoRuletaRusa(const TipoRayo tipoRayo, const Punto& origen, const Direccion& wo,
                           const Direccion& normal, float& probDirRayo) {
    Rayo wi;
    wi.o = origen;
    probDirRayo = 1.0f;
    if (tipoRayo == DIFUSO) {
        wi.d = generarDireccionAleatoria(normal, probDirRayo);
    } else if (tipoRayo == ESPECULAR) {
        wi.d = calcDirEspecular(wo, normal);
    } else if (tipoRayo == REFRACTANTE){
        auto wt = calcDirRefractante(wo, normal, 1.0f, 1.5f);
        if (wt) {       // Hay refracción
            wi.d = *wt;
        } else {        // Reflexión interna total
            wi.d = calcDirEspecular(wo, normal);
        }
    }
    return wi;
}

TipoRayo dispararRuletaRusa(const BSDFs& coefs, float& probTipoRayo) {
    float maxKD = max(coefs.kd);
    float maxKS = max(coefs.ks);
    float maxKT = max(coefs.kt);
    float total = 1.0f;

    float probDifuso = maxKD / total;
    float probEspecular = maxKS / total;
    float probRefractante = maxKT / total;
    //float probAbsorbente = 1.0f - probDifuso - probEspecular - probRefractante;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float bala = dist(gen);     // Random float entre (0,1)

    if (bala <= probDifuso) {
        probTipoRayo = probDifuso;
        return DIFUSO;  // Rayo difuso
    } else if (bala <= probDifuso + probEspecular) {
        probTipoRayo = probEspecular;
        return ESPECULAR;  // Rayo especular
    } else if (bala <= probDifuso + probEspecular + probRefractante) {
        probTipoRayo = probRefractante;
        return REFRACTANTE;  // Rayo refractante
    } else {
        return ABSORBENTE;   // Absorbente
    }
}

RGB calcBrdfDifusa(const RGB& kd, Primitiva* objeto, const Punto& p0){
    //return kd / M_PI;
    if (objeto->tengoTextura()) return objeto->k_d(p0) / M_PI;
    return kd / M_PI;
}

RGB calcBrdfEspecular(const RGB& ks) {
    return ks;
}

RGB calcBtdf(const RGB& kt) {
    return kt;
}

RGB calcBsdf(const BSDFs& coefs, TipoRayo tipoRayo, Primitiva* objeto, const Punto& p0) {
    RGB resultado;
    
    switch (tipoRayo) {
        case DIFUSO:
            resultado = calcBrdfDifusa(coefs.kd, objeto, p0);
            break;
        
        case ESPECULAR:
            resultado = calcBrdfEspecular(coefs.ks);
            break;
        
        case REFRACTANTE:
            resultado = calcBtdf(coefs.kt);
            break;
            
        case ABSORBENTE:
            resultado = RGB(0.0f, 0.0f, 0.0f);
            break;
            
        default:
            cerr << "Error: rayo no identificado" << endl;
            break;
    }
    
    return resultado;
}

float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n){
    return abs(dot(n, d / modulo(d)));
}

RGB nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                        const RGB& kd, Primitiva* objOrigen) {
    RGB radianciaSaliente(0.0f, 0.0f, 0.0f);
    for (LuzPuntual luz : escena.luces) {
        if (!escena.luzIluminaPunto(p0, luz)) {
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }

        Direccion dirIncidente = luz.c - p0;
        float cosAnguloIncidencia = calcCosenoAnguloIncidencia(normalizar(dirIncidente), normal);
        RGB reflectanciaBrdfDifusa = calcBrdfDifusa(kd, objOrigen, p0);
        RGB radianciaIncidente = luz.p / (modulo(dirIncidente) * modulo(dirIncidente));
        radianciaIncidente = radianciaIncidente * (reflectanciaBrdfDifusa * cosAnguloIncidencia);
        
        radianciaSaliente += radianciaIncidente;
    }
    
    int num_luces = 0;
    
    for (const Primitiva* objeto : escena.primitivas) {   // Iteramos por luces de área
        Punto origenLuz;
        float prob;
        if (!(objeto->soyFuenteDeLuz()) || !escena.luzIluminaPunto(p0, objeto, origenLuz, prob)) {
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }
        num_luces += 1;
        RGB powerLuzArea = objeto->power;
        Direccion dirIncidente = origenLuz - p0;
        float distanciaCuadrado = modulo(dirIncidente) * modulo(dirIncidente);
        distanciaCuadrado = max(distanciaCuadrado, LIMITE_DISTANCIA_RAYO);  // evitamos un valor explosivo
        
        float cosAnguloIncidencia = calcCosenoAnguloIncidencia(normalizar(dirIncidente), normal);
        RGB reflectanciaBRDFDifusa = calcBrdfDifusa(kd, objOrigen, p0);
        float cosNLuzWiLuz = calcCosenoAnguloIncidencia(normalizar(-dirIncidente), objeto->getNormal(origenLuz));
        RGB radianciaIncidente = (powerLuzArea * reflectanciaBRDFDifusa * cosNLuzWiLuz * cosAnguloIncidencia)
                                 / (distanciaCuadrado * prob);

        radianciaSaliente += radianciaIncidente;
    }
    
    num_luces = max(num_luces, 1);
    return radianciaSaliente / num_luces;
}

RGB recursividadRadianciaIndirecta(const Punto& origen, const Direccion &wo, const BSDFs &coefsOrigen, 
                                   const Direccion& normal, const Escena& escena,
                                   const unsigned rebotesRestantes, Primitiva* objOrigen) {
    if (rebotesRestantes == 0) {     // TERMINAL: alcanzado max rebotes
        return RGB({0.0f, 0.0f, 0.0f});
    }
    
    RGB powerLuzArea;
    if (escena.puntoPerteneceALuz(origen, powerLuzArea)) {   // TERMINAL: somos una fuente de luz
        // if (woEsDifuso) return RGB({0.0f, 0.0f, 0.0f});  // con esto objetos cerca de luz sangran negro
        return powerLuzArea;
    }
    
    float probTipoRayo;
    TipoRayo tipoRayo = dispararRuletaRusa(coefsOrigen, probTipoRayo);
    if (tipoRayo == ABSORBENTE) {       // TERMINAL: rayo absorbente
        return RGB({0.0f, 0.0f, 0.0f});
    }

    float probDirRayo;     // Ojo! La probabilidad es para la siguiente llamada recursiva pq es wi, no wo
    Rayo wi = obtenerRayoRuletaRusa(tipoRayo, origen, wo, normal, probDirRayo);
    
    //bool flagEventoDifuso = false;
    RGB radianciaSalienteDirecta(0.0f, 0.0f, 0.0f);
    if (tipoRayo == DIFUSO) {
        radianciaSalienteDirecta = nextEventEstimation(origen, normal, escena, coefsOrigen.kd, objOrigen);
    }
    
    BSDFs coefsPtoIntersec;
    Punto ptoIntersec;
    Direccion nuevaNormal;
    Primitiva* nuevoObjIntersecado = nullptr;
    bool hayIntersec = escena.interseccion(wi, coefsPtoIntersec, ptoIntersec, nuevaNormal, &nuevoObjIntersecado);

    if (!hayIntersec) {    // TERMINAL: wi no choca contra nada, devolvemos radiancia de pto actual
        return radianciaSalienteDirecta / probTipoRayo;
    }
    
    // --- Ecuación de render ---
    float coseno;
    if (tipoRayo == DIFUSO) {
        // Compensa la prob. de que se elija esa dirección del rayo difuso de entre todos los del hemisferio
        coseno = calcCosenoAnguloIncidencia(wi.d, normal);
    } else {  // tipoRayo == ESPECULAR || tipoRayo == REFRACTANTE
        coseno = 1.0f;      // El coseno se anula porque el rayo solamente puede ir en una dirección
    }
    RGB bsdf = calcBsdf(coefsOrigen, tipoRayo, objOrigen, origen);
    RGB radianciaSalienteIndirecta = recursividadRadianciaIndirecta(ptoIntersec, wi.d, coefsPtoIntersec,
                                                                    nuevaNormal, escena, rebotesRestantes - 1,
                                                                    nuevoObjIntersecado);

    radianciaSalienteIndirecta = (radianciaSalienteIndirecta / probDirRayo) * bsdf * coseno;
    return (radianciaSalienteDirecta + radianciaSalienteIndirecta) / probTipoRayo;
           
}

RGB obtenerRadianciaSalienteIndirecta(const Escena& escena, const unsigned maxRebotes, 
                                      const unsigned numRayosMontecarlo, const Punto& ptoIntersec,
                                      const Direccion& wo, const BSDFs &coefsPtoInterseccion,
                                      const Direccion& normal, Primitiva* objOrigen) {
    RGB sumaRadianciasIndirectas(0.0f, 0.0f, 0.0f);
    for (unsigned i = 0; i < numRayosMontecarlo; ++i) {   // Emisión media de N rayos por Montecarlo
        sumaRadianciasIndirectas += recursividadRadianciaIndirecta(ptoIntersec, wo, coefsPtoInterseccion,
                                                                   normal, escena, maxRebotes, objOrigen);
    }
    return sumaRadianciasIndirectas / numRayosMontecarlo;       // Media de todas
}

RGB obtenerRadianciaSalienteTotal(const Rayo &rayoIncidente, const Escena &escena, const unsigned maxRebotes, 
                                  const unsigned numRayosMontecarlo){
    Punto ptoIntersec;
    Direccion normal;
    RGB radianciaSalienteDirecta(0.0f, 0.0f, 0.0f);
    RGB radianciaSalienteIndirecta(0.0f, 0.0f, 0.0f);
    BSDFs coefsPtoInterseccion;
    Primitiva* objIntersecado = nullptr;
    
    if (escena.interseccion(rayoIncidente, coefsPtoInterseccion, ptoIntersec, normal, &objIntersecado)) {
        RGB powerLuzArea;
        if (escena.puntoPerteneceALuz(ptoIntersec, powerLuzArea)) {  // 1º Rayo (de la cam) choca con luz de área
            return powerLuzArea;
        }
        
        if (maxRebotes == 0) {  // Solo luz directa
            radianciaSalienteDirecta = nextEventEstimation(ptoIntersec, normal, escena,
                                                           coefsPtoInterseccion.kd, objIntersecado);
        } else {    // Luz directa e indirecta
            radianciaSalienteIndirecta = obtenerRadianciaSalienteIndirecta(escena, maxRebotes,
                                                                           numRayosMontecarlo, ptoIntersec,
                                                                           rayoIncidente.d,
                                                                           coefsPtoInterseccion, normal,
                                                                           objIntersecado);
        }
    }
    return radianciaSalienteDirecta + radianciaSalienteIndirecta;
}

void renderizarEscena1RPP(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const unsigned totalPixeles,
                          const bool printPixelesProcesados) {
    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            //if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);
            
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            coloresEscena[alto][ancho] = obtenerRadianciaSalienteTotal(rayo, escena, maxRebotes, numRayosMontecarlo);
        }
        pixelesProcesados += numPxlsAlto;
        if (printPixelesProcesados) cout << "Progreso: " << pixelesProcesados
            << " / " << totalPixeles << " pixeles procesados." << endl;
    }
}

void renderizarEscenaConAntialiasing(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const bool printPixelesProcesados,
                          const unsigned totalPixeles, const unsigned rpp) {
    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            //if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);
            
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB radianciaSalienteTotal;
            for (unsigned i = 0; i < rpp; ++i) {
                rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                radianciaSalienteTotal += obtenerRadianciaSalienteTotal(rayo, escena, maxRebotes, numRayosMontecarlo);
            }
            coloresEscena[alto][ancho] = radianciaSalienteTotal / rpp;
        }
        pixelesProcesados += numPxlsAlto;
        if (printPixelesProcesados) cout << "Progreso: " << pixelesProcesados
            << " / " << totalPixeles << " pixeles procesados." << endl;
    }
}


void printPixelActual(unsigned totalPixeles, unsigned numPxlsAncho, unsigned ancho, unsigned alto){
    unsigned pixelActual = numPxlsAncho * ancho + alto + 1;
    if (pixelActual % 100 == 0 || pixelActual == totalPixeles) {
        cout << pixelActual << " pixeles de " << totalPixeles << endl;
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

void renderizarRangoFilas(Camara& camara, unsigned inicioFila, unsigned finFila,
                          unsigned numPxlsAncho, const Escena& escena, float anchoPorPixel,
                          float altoPorPixel, const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const unsigned rpp, const unsigned thread,
                          const bool printPixelesProcesados, const unsigned totalPixeles) {
    for (unsigned alto = inicioFila; alto < finFila; ++alto) {
        for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB radianciaSalienteTotal;
            if (rpp == 1) {  // Sin antialiasing
                rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                coloresEscena[alto][ancho] = obtenerRadianciaSalienteTotal(rayo, escena, maxRebotes, numRayosMontecarlo);
            } else {  // Con antialiasing
                for (unsigned i = 0; i < rpp; ++i) {
                    rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                    globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                    radianciaSalienteTotal += obtenerRadianciaSalienteTotal(rayo, escena, maxRebotes, numRayosMontecarlo);
                }
                coloresEscena[alto][ancho] = radianciaSalienteTotal / rpp;
            }
        }
        pixelesProcesados += numPxlsAncho;
        if (printPixelesProcesados) cout << "Progreso: " << pixelesProcesados
            << " / " << totalPixeles << " pixeles procesados." << endl;
    }
}

void renderizarEscenaConThreads(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                                const Escena& escena, const string& nombreEscena, const unsigned rpp,
                                const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                                const bool printPixelesProcesados, unsigned numThreads) {
    pixelesProcesados = 0;
    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
    
    unsigned totalPixeles = numPxlsAlto * numPxlsAncho;
    cout << "Numero de threads a usar: " << numThreads << endl;
    if (printPixelesProcesados) cout << "Procesando pixeles..." << endl << "0 pixeles de " << totalPixeles << endl;

    // Inicializado todo a color negro
    vector<vector<RGB>> coloresEscena(numPxlsAlto, vector<RGB>(numPxlsAncho, {0.0f, 0.0f, 0.0f}));

    unsigned filasPorThread = numPxlsAlto / numThreads;
    unsigned filasRestantes = numPxlsAlto % numThreads;

    std::vector<std::thread> threads;
    unsigned inicioFila = 0;

    for (unsigned t = 0; t < numThreads; ++t) {
        // Distribuir filas extra entre los primeros threads
        unsigned finFila = inicioFila + filasPorThread + (t < filasRestantes ? 1 : 0);
        threads.emplace_back(renderizarRangoFilas, std::ref(camara), inicioFila, finFila, numPxlsAncho,
                             std::ref(escena), anchoPorPixel, altoPorPixel, maxRebotes, numRayosMontecarlo,
                             std::ref(coloresEscena), rpp, t, printPixelesProcesados, totalPixeles);
        inicioFila = finFila;
    }

    for (auto& thread : threads) {  // Esperar a que todos los threads terminen
        thread.join();
    }

    pintarEscenaEnPPM(nombreEscena, coloresEscena);
}
