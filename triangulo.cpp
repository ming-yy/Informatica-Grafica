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

Triangulo::Triangulo(Punto& _v0, Punto& _v1, Punto& _v2,
                     array<float, 3> _emision = {0.0f, 0.0f, 0.0f}):
                     v0(_v0), v1(_v1), v2(_v2), emision(_emision) {}


bool Triangulo::interseccionRayoTriangulo(Punto& origen, Direccion& direccion,
                                          Punto& puntoInterseccion) {
    const float EPSILON = 1e-6f;

    Direccion edge1 = v1 - v0;
    Direccion edge2 = v2 - v0;
    Direccion h = cross(direccion, edge2);
    float a = dot(edge1, h);

    if (fabs(a) < EPSILON) {
        cout << "No hay intersección, el rayo es paralelo al triángulo." << endl;
        return false;
    }

    float f = 1.0f / a;
    Direccion s = origen - v0;
    float u = f * dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        return false; // La intersección está fuera del triángulo
    }

    Direccion q = cross(s, edge1);
    float v = f * dot(direccion, q);

    if (v < 0.0f || u + v > 1.0f) {
        return false; // La intersección está fuera del triángulo
    }

    float t = f * dot(edge2, q);
    if (t > EPSILON) {
        puntoInterseccion = origen + direccion * t; // Calcular el punto de intersección
        return true;
    } else {
        return false; // No hay intersección en la dirección del rayo
    }
}
