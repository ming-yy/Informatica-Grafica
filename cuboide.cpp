//*****************************************************************
// File:   cuboide.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   enero 2025
// Coms:   Práctica 3.3 de Informática Gráfica
//*****************************************************************

#include "cuboide.h"


Cuboide::Cuboide(const vector<Plano>& p): Primitiva() {
    if (p.size() != 6) {
        throw std::invalid_argument("ERROR: Un cuboide debe estar definido por exactamente 6 planos.");
    }
    this->planos = p;
}

RGB Cuboide::kd(const Punto& p) const {
    for (const auto& plano : this->planos) {
        if (plano.pertenece(p)) {
            return plano.kd(p);
        }
    }
    throw std::invalid_argument("El punto no está en la superficie del cuboide.");
}

void Cuboide::interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const {
    BSDFs aux;
    //int i = 0;
    for (const auto& plano : this->planos) {
        vector<Punto> interseccionesAux;
        plano.interseccion(rayo, interseccionesAux, aux);
        if (!interseccionesAux.empty()) {
            bool dentro = true;
            
            // Comprobamos si el rayo <rayo>, tras intersecar con <plano>, interseca con otro plano en el
            // sentido del vector otroPlano.normal (está bien) o en su sentido opuesto (está mal, rayo no válido)
            for (const auto& otroPlano : this->planos) {
                if (&plano != &otroPlano && (otroPlano.distancia(interseccionesAux[0]) < MARGEN_ERROR)) {
                    dentro = false;
                    break;
                }
            }
            if (dentro) {
                //cout << "Plano :" << i << endl;
                ptos.push_back(interseccionesAux[0]);
            }
        }
        //i++;
    }
    
    // Ordenar los puntos de intersección por su distancia al origen del rayo
    std::sort(ptos.begin(), ptos.end(), [&rayo](const Punto& p1, const Punto& p2) {
        double d1 = modulo(p1 - rayo.o);
        double d2 = modulo(p2 - rayo.o);
        return d1 < d2;
    });
}

bool Cuboide::pertenece(const Punto& punto) const {
    for (const auto& plano : this->planos) {
        if (plano.distancia(punto) > MARGEN_ERROR) {
            return false; // Si el punto está fuera de algún plano, no pertenece al cuboide
        }
    }
    return true;
}

Direccion Cuboide::getNormal(const Punto& punto) const {
    //int i = 0;
    for (const auto& plano : this->planos) {
        //cout << "i = " << i << endl;
        if (plano.pertenece(punto)) {
            return plano.getNormal(punto);
        }
        //i++;
    }
    throw std::invalid_argument("El punto no está en la superficie del cuboide.");
}

bool Cuboide::puntoEsFuenteDeLuz(const Punto& punto) const {
    return false;
}

Punto Cuboide::generarPuntoAleatorio(float& prob) const {
    return Punto();
}


float Cuboide::getEjeTexturaU(const Punto& pto) const {
    for (const auto& plano : this->planos) {
        if (plano.pertenece(pto)) {
            return plano.getEjeTexturaU(pto);
        }
    }
    throw std::runtime_error("Error: punto no pertenece al objeto. NO debería pasar esto.");
    return 0.0f;
}

float Cuboide::getEjeTexturaV(const Punto& pto) const {
    for (const auto& plano : this->planos) {
        if (plano.pertenece(pto)) {
            return plano.getEjeTexturaV(pto);
        }
    }
    throw std::runtime_error("Error: punto no pertenece al objeto. NO debería pasar esto.");
    return 0.0f;
}


void Cuboide::diHola() const {
    
}

