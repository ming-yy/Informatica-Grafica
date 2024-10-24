//*****************************************************************
// File:   triangulo.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "triangulo.h"

using std::cout;
using std::endl;


Triangulo::Triangulo() : v0(Punto()), v1(Punto()), v2(Punto()),
                         emision({0.0f, 0.0f, 0.0f}) {}

Triangulo::Triangulo(const Punto& _v0, const Punto& _v1, const Punto& _v2,
                     array<float, 3> _emision): v0(_v0), v1(_v1), v2(_v2),
                                                emision(_emision) {}

void Triangulo::interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                         std::array<float, 3>& emision) const {

    const float EPSILON = 1e-6f;

    Direccion edge1 = v1 - v0;
    Direccion edge2 = v2 - v0;
    Direccion h = cross(rayo.d, edge2);
    float a = dot(edge1, h);

    if (fabs(a) < EPSILON) {
        cout << "No hay intersección, el rayo es paralelo al triángulo." << endl;
        return;
    }

    float f = 1.0f / a;
    Direccion s = rayo.o - v0;
    float u = f * dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        return; // La intersección está fuera del triángulo
    }

    Direccion q = cross(s, edge1);
    float v = f * dot(rayo.d, q);

    if (v < 0.0f || u + v > 1.0f) {
        return; // La intersección está fuera del triángulo
    }

    float t = f * dot(edge2, q);
    if (t > EPSILON) {
        ptos.push_back(Punto(rayo.o + rayo.d * t)); // Calcular el punto de intersección
        return;
    } else {
        return ; // No hay intersección en la dirección del rayo
    }
}
