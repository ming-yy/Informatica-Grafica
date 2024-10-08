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


Plano::Plano(): c(0.0f, 0.0f, 0.0f), n(0.0f, 0.0f, 0.0f), d(0.0f){}

Plano::Plano(Punto& _c, Direccion& _n, float _d): c(_c), n(_n), d(_d){}


bool interseccionRayoPlano(Punto& p, Direccion& d, Plano& e, Punto& puntoInterseccion) {
    float denominador = dot(d, e.n);
    if (fabs(denominador) < 1e-6f) {    // Para evitar problemas de imprecision
        cout << "No hay intersección, el rayo es paralelo al plano." << endl;
        return false;
    }
    
    float numerador = (-1) * (e.d + dot(p, e.n));
    float t = numerador / denominador;
    if (t < 0) {
        cout << "No hay intersección en la dirección positiva del rayo." << endl;
        return false;
    }
    
    Punto aux = p + d * t;
    // DEBUG
    cout << "Numerador: " << numerador << endl;
    cout << "Denominador: " << denominador << endl;
    cout << "t: " << t << endl;
    cout << "Hay interseccion en el punto: " << aux << endl;
    return true;
}

ostream& operator<<(ostream& os, const Plano& r)
{
    os << "Centro: " << r.c << "\nNormal: " << r.n << "\nDistancia: " << r.d;
    return os;
}
