//*****************************************************************
// File:   esfera.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "esfera.h"

#define MARGEN_ERROR 10e-6f

Esfera::Esfera(): centro(Punto()), radio(0.0f), emision({0.0f,0.0f,0.0f}), soyLuz(false) {}

Esfera::Esfera(const Punto& _centro, const float& _radio, const RGB& _emision,
               const bool _soyLuz) :
               centro(_centro), radio(_radio), emision(_emision), soyLuz(_soyLuz) {}

Esfera::Esfera(const Planeta& p) : centro(p.centro), radio(p.radio), emision(p.emision),
                                   soyLuz(false){}

void Esfera::interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                          RGB& emision, bool& choqueConLuz) const {
    float a = modulo(rayo.d);
    a *= a;
    float b = 2 * dot(rayo.d, rayo.o - this->centro);
    float c = (modulo(rayo.o - this->centro) * modulo(rayo.o - this->centro))
                - (this->radio * this->radio);
    
    // DEBUG
    //cout << "a = " << a << "\nb = " << b << "\nc = " << c << endl;
    
    float discriminante = b * b - 4 * a * c;    // Cálculo del discriminante
    if (discriminante > 0) {    // Caso de soluciones reales
        float t1 = (-b + sqrt(discriminante)) / (2 * a);
        float t2 = (-b - sqrt(discriminante)) / (2 * a);
        Punto p1 = rayo.o + rayo.d * t1;
        Punto p2 = rayo.o + rayo.d * t2;
        if (t1 > MARGEN_ERROR && t2 > MARGEN_ERROR) {
            if (modulo(rayo.o - p1) < modulo(rayo.o - p2)) {    // Primero interseca con p1
                ptos.push_back(p1);
                ptos.push_back(p2);
            } else {             // Primero interseca con p2
                ptos.push_back(p2);
                ptos.push_back(p1);
            }
            emision = this->emision;
            choqueConLuz = soyLuz;
        } else if (t1 > MARGEN_ERROR) {
            ptos.push_back(p1);
            emision = this->emision;
            choqueConLuz = soyLuz;
        } else if (t2 > MARGEN_ERROR) {
            ptos.push_back(p2);
            emision = this->emision;
            choqueConLuz = soyLuz;
        }
 
        // DEBUG
        //cout << "Hay 2 puntos de interseccion: " << endl;
        //cout << "t1 = " << t1 << " --> " << p1 << endl;
        //cout << "t2 = " << t2 << " --> " << p2 << endl;
        //cout << "Primero intersecara con: ";
        //cout << puntoInterseccion << endl;
    } else if (discriminante == 0) {  // Caso de una solución única
        float t = -b / (2 * a);
        Punto puntoInterseccion = rayo.o + rayo.d * t;
        if (t > MARGEN_ERROR) {
            ptos.push_back(puntoInterseccion);
            emision = this->emision;
            choqueConLuz = soyLuz;
        }
        // DEBUG
        //cout << "Hay 1 punto de interseccion (tngente): " << endl;
        //cout << "t = " << t << " --> " << puntoInterseccion << endl;
    }
    //else {  // Caso no hay solución real (soluciones complejas)
        // DEBUG
        //cout << "No hay puntos de intersección!" << endl;
        //resVal = false;
    //}
}

Direccion Esfera::getNormal(const Punto& punto) const {
    return normalizar(punto - centro);
}

bool Esfera::soyFuenteDeLuz() const {
    return this->soyLuz;
}

Punto Esfera::generarPuntoAleatorio() const {
    return Punto();
}
