//*****************************************************************
// File:   escena.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "escena.h"
#include <memory>

Escena::Escena(): primitivas(vector<Primitiva*>()) {}

Escena::Escena(vector<Primitiva*> _primitivas, vector<LuzPuntual> _luces):
               primitivas(_primitivas), luces(_luces) {}


bool Escena::interseccion(const Rayo& rayo, BSDFs& coefsObjeto, Punto& ptoMasCerca,
                          Direccion& normal) const {
    bool resVal = false;
    bool primerIntersec = true;  // Flag: la primera intersección encontrada

    for (const Primitiva* objeto : this->primitivas) {
        vector<Punto> interseciones;
        BSDFs coefsAux;

        objeto->interseccion(rayo, interseciones, coefsAux);
        if (!interseciones.empty()) {
            resVal = true;
            // El intersec[0] es el punto más cercano al origen del rayo en este objeto
            if (primerIntersec || (modulo(rayo.o - interseciones[0]) < modulo(rayo.o - ptoMasCerca))) {
                ptoMasCerca = interseciones[0];
                normal = objeto->getNormal(ptoMasCerca);
                coefsObjeto = coefsAux;
                primerIntersec = false;
            }
        }
    }

    return resVal;
}

#include "esfera.h"

bool Escena::puntoPerteneceALuz(const Punto& p0, RGB& powerLuzArea, bool debug) const {
    bool resVal = false;
    bool aux = false;
    for (Primitiva* primitiva : this->primitivas) {
        if (debug) {
            Esfera* esfera = dynamic_cast<Esfera*>(primitiva);
            if (esfera) {
                //esfera->diHola(); // Éxito: figura es realmente una Esfera
                aux = true;
            } else {
                //cout << "El objeto no es una Esfera" << endl;
            }
            
            array<float, 3> xxd = {-0.154196858, -0.179686651, -0.184219122};
            
            if (aux && p0.coord[0] == xxd[0] && p0.coord[1] == xxd[1] && p0.coord[2] == xxd[2]) {
                //cout << "pertenece: " << esfera->pertenece(p0) << endl;
                Punto pto(0.0f, 0.0f, 0.0f);
                float distancia = modulo(pto - p0);
                float total = abs(distancia - 0.3f);
                
                cout << "   Esfera es fuente: " << primitiva->soyFuenteDeLuz() << ", distancia: " << distancia
                    << ", diferencia: " << total << ", pertenece: " << (total <= MARGEN_ERROR) << endl;
            }
            
            if (primitiva->puntoEsFuenteDeLuz(p0)) {
                resVal = true;
                array<float, 3> xd = {-0.16, -0.202272, -0.150654};

                if (p0.coord[0] >= xd[0]) {
                    powerLuzArea = RGB({0.0f, 0.0f, 1.0f});
                    cout << "     ++++++ AZUL ++++++ Pto: " << p0 << endl << endl;
                } else {
                    //powerLuzArea = RGB({0.0f, 0.0f, 1.0f});
                    powerLuzArea = primitiva->power;
                    cout << "     ++++++Pto es fuente++++++ Pto: " << p0 << endl << endl;
                }
                break;
            } else if (aux) {
                cout << "------Pto " << p0 << " debe ser fuente y no lo es." << endl;
                cout << "Esfera dice: " << esfera->pertenece(p0) << endl << endl;
            }
        } else {
            if (primitiva->puntoEsFuenteDeLuz(p0)) {
                resVal = true;
                powerLuzArea = primitiva->power;
                break;
            }
        }
    }
    return resVal;
}

bool Escena::luzIluminaPunto(const Punto& p0, const LuzPuntual& luz) const {
    bool iluminar = true;
    Direccion d = normalizar(luz.c - p0);
    Punto ptoMasCerca;
    BSDFs coefs;
    Direccion normal;
    bool chocaObjeto = this->interseccion(Rayo(d, p0), coefs, ptoMasCerca, normal);
    
    if (chocaObjeto) {
        iluminar = modulo(luz.c - p0) <= modulo(ptoMasCerca - p0);
    }
    
    return iluminar;
}

bool Escena::luzIluminaPunto(const Punto& p0, const Primitiva* luz, Punto& origenLuz, float& prob) const {
    bool iluminar = false;
    int numIters = NUM_MUESTRAS_LUZ_AREA;     // Tiene que ir en función del tamaño del plano
    for (int i = 0; i < numIters && !iluminar; ++i) {
        Punto ptoRandom = luz->generarPuntoAleatorio(prob);
        Direccion d = normalizar(ptoRandom - p0);
        Punto ptoMasCerca;
        BSDFs coefs;
        Direccion normal;
        // Rayo desde punto a iluminar (p0) --> ptoRandom de la luz
        bool chocaObjeto = this->interseccion(Rayo(d, p0), coefs, ptoMasCerca, normal);
        
        if (chocaObjeto) {  // Miramos si el objeto más cercano contra el que choca es esa luz en <ptoRandom>
            iluminar = modulo(ptoRandom - p0) <= modulo(ptoMasCerca - p0);
            origenLuz = ptoRandom;
        }
    }
    
    return iluminar;
}

bool Escena::puntoIluminado(const Punto& p0) const {
    for(const LuzPuntual& luz : this->luces) {
        if (luzIluminaPunto(p0, luz)) return true;
    }
    
    Punto origenLuz;
    float prob;
    for(auto objeto : this->primitivas) {
        if (luzIluminaPunto(p0, objeto, origenLuz, prob)) return true;
    }
    
    return false;
}
