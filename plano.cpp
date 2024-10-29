//*****************************************************************
// File:   plano.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "plano.h"

using std::ostream;
using std::cout;
using std::endl;
using std::vector;


Plano::Plano(): n(0.0f, 0.0f, 0.0f), d(0.0f), emision({0.0f, 0.0f, 0.0f}) {}

Plano::Plano(const Direccion& _n, float _d, const RGB& _emision):
             n(_n), d(_d), emision(_emision) {}

void Plano::interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                        RGB& emision) const {
    float denominador = dot(rayo.d, n);
    if (fabs(denominador) < 1e-6f) {    // Para evitar problemas de imprecision
        //cout << "No hay intersección, el rayo es paralelo al plano." << endl;
        return;
    }
    
    float numerador = (-1) * (d + dot(rayo.o, n));
    float t = numerador / denominador;
    if (t < 0) {
        //cout << "No hay intersección en la dirección positiva del rayo." << endl;
        return;
    }
    
    Punto aux = rayo.o + rayo.d * t;
    ptos.push_back(aux);
    emision = this->emision;
    
    // DEBUG
    // cout << "Numerador: " << numerador << endl;
    // cout << "Denominador: " << denominador << endl;
    // cout << "t: " << t << endl;
    // cout << "Hay interseccion en el punto: " << aux << endl;
    
}

Direccion Plano::getNormal(const Punto& punto) const {
    return normalizar(n);
}

ostream& operator<<(ostream& os, const Plano& r)
{
    os << "\nNormal: " << r.n << "\nDistancia: " << r.d;
    return os;
}
