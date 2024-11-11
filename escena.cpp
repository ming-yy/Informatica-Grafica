//*****************************************************************
// File:   escena.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "escena.h"
#include <memory>

Escena::Escena(): primitivas(std::vector<Primitiva*>()) {}

Escena::Escena(std::vector<Primitiva*> _primitivas, std::vector<LuzPuntual> _luces):
                primitivas(_primitivas), luces(_luces) {}


bool Escena::interseccion(const Rayo& rayo, RGB& resEmision, Punto& ptoMasCerca, Direccion& normal,
                          bool& choqueConLuz) const {
    bool resVal = false;
    bool primerIntersec = true;  // Flag: la primera intersección encontrada
    bool auxChoqueConLuz = false;

    for (const Primitiva* objeto : this->primitivas) {
        std::vector<Punto> intersec;
        RGB emision;

        objeto->interseccion(rayo, intersec, emision, auxChoqueConLuz);
        if (!intersec.empty()) {    // Hay intersección con el objeto <objeto>
            resVal = true;
            // El intersec[0] es el punto más cercano al origen del rayo en este objeto
            if (primerIntersec || (modulo(rayo.o - intersec[0]) < modulo(rayo.o - ptoMasCerca))) {
                ptoMasCerca = intersec[0];
                choqueConLuz = auxChoqueConLuz;
                normal = objeto->getNormal(ptoMasCerca);
                resEmision = emision;
                primerIntersec = false;  // Marcamos que ya se encontró una intersección
            }
        }
    }

    return resVal;
}

bool Escena::luzIluminaPunto(const Punto& p0, const LuzPuntual& luz) const {
    bool iluminar = true;
    Direccion d = normalizar(luz.c - p0);
    Punto ptoMasCerca;
    RGB rgb;
    Direccion normal;
    bool choqueConLuz = false;
    bool chocaObjeto = this->interseccion(Rayo(d, p0), rgb, ptoMasCerca, normal, choqueConLuz);
    
    if (chocaObjeto) {
        iluminar = modulo(luz.c - p0) <= modulo(ptoMasCerca - p0);
    }
    
    // DEBUG
    /*
    else {
        std::cout << "Choca y no ilumina" << "\n"
        << "P0: " << p0 << "\n"
        << "Luz: " << luz.c << "\n"
        << "distancia luz-p0: " << modulo(luz.c - p0) << "\n"
        << "Pto más cerca: " << ptoMasCerca << "\n"
        << "distancia ptoMasCerca-p0: " << modulo(ptoMasCerca - p0) << endl;
    }
    */
    
    return iluminar;
}

bool Escena::luzIluminaPunto(const Punto& p0, const Primitiva* luz, Punto& origen) const {
    bool iluminar = false;
    int numIters = 100;     // Tiene que ir en función del tamaño del plano
    for (int i = 0; i < numIters && !iluminar; ++i) {
        Punto ptoLuz = luz->generarPuntoAleatorio();
        Direccion d = normalizar(ptoLuz - p0);
        Punto ptoMasCerca;
        RGB rgb;
        Direccion normal;
        bool choqueConLuz = false;
        bool chocaObjeto = this->interseccion(Rayo(d, p0), rgb, ptoMasCerca, normal, choqueConLuz);
        
        if (chocaObjeto) {
            iluminar = modulo(ptoLuz - p0) <= modulo(ptoMasCerca - p0);
            origen = ptoLuz;
        }
    }
    
    return iluminar;
}

bool Escena::puntoIluminado(const Punto& p0) const {
    bool iluminar = true;
    for(const LuzPuntual& luz : this->luces) {
        iluminar = luzIluminaPunto(p0, luz);
        if (iluminar) break;
    }
    /*
    for(const Primitiva& objeto : this->primitivas) {
        iluminar = luzIluminaPunto(p0, objeto);
        if (iluminar) break;
    }
    */
    return iluminar;
}
