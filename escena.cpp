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


bool Escena::interseccion(const Rayo& rayo, RGB& resEmision, Punto& ptoMasCerca, Direccion& normal) const {
    bool resVal = false;
    bool primerIntersec = true;  // Flag: la primera intersección encontrada

    for (const Primitiva* objeto : this->primitivas) {
        std::vector<Punto> intersec;
        RGB emision;

        objeto->interseccion(rayo, intersec, emision);

        if (!intersec.empty()) {    // Si hay intersecciones
            resVal = true;
            // El intersec[0] es el punto más cercano al origen del rayo en este objeto
            if (primerIntersec || (modulo(rayo.o - intersec[0]) < modulo(rayo.o - ptoMasCerca))) {
                ptoMasCerca = intersec[0];
                normal = objeto->getNormal(ptoMasCerca);
                resEmision = emision;
                primerIntersec = false;  // Marcamos que ya se encontró una intersección
            }
        }
    }

    return resVal;
}


bool Escena::puntoIluminado(const Punto& p0) const {
    bool iluminar = true;
    for(const LuzPuntual& luz : this->luces) {
        Direccion d = normalizar(luz.c - p0);
        Punto ptoMasCerca;
        RGB rgb;
        Direccion normal;
        bool chocaObjeto = this->interseccion(Rayo(d, p0), rgb, ptoMasCerca, normal);
        
        if (chocaObjeto) {
            iluminar = modulo(luz.c - p0) <= modulo(ptoMasCerca - p0);
        }
        
        if (iluminar) break;
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
                    
    }
    return iluminar;
}
