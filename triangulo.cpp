//*****************************************************************
// File:   triangulo.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include <random>
#include "triangulo.h"
#include "utilidades.h"


Triangulo::Triangulo() : Primitiva(), v0(Punto()), v1(Punto()), v2(Punto()) {}

Triangulo::Triangulo(const Punto& _v0, const Punto& _v1, const Punto& _v2, const RGB& _reflectancia,
                     const string _material, const RGB& _power):
                     Primitiva(_reflectancia, _material, _power), v0(_v0), v1(_v1), v2(_v2){}

void Triangulo::interseccion(const Rayo& rayo, vector<Punto>& ptos,
                             BSDFs& coefs, RGB& powerLuzArea) const {
    Direccion edge1 = v1 - v0;
    Direccion edge2 = v2 - v0;
    Direccion h = cross(rayo.d, edge2);
    float a = dot(edge1, h);

    if (fabs(a) < MARGEN_ERROR) {
        // cout << "No hay intersección, el rayo es paralelo al triángulo." << endl;
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
    if (t > MARGEN_ERROR) {
        ptos.push_back(Punto(rayo.o + rayo.d * t));
        coefs = this->coeficientes;
        powerLuzArea = this->power;
    } //else {
        //return ; // No hay intersección en la dirección del rayo
    //}
}

bool Triangulo::pertenece(const Punto& p0) const {
    return false;
}

Direccion Triangulo::getNormal(const Punto& punto) const {
    Direccion d1 = v0 - v1;
    Direccion d2 = v1 - v2;
    return normalizar(cross(d1, d2));
}

Punto Triangulo::generarPuntoAleatorio(float& prob) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float r1 = dist(gen);
    float r2 = dist(gen);

    // r1 + r2 <= 1.0 (si no, reflejamos el punto en el triángulo)
    if (r1 + r2 > 1.0f) {
        r1 = 1.0f - r1;
        r2 = 1.0f - r2;
    }

    // Coordenadas baricéntricas
    float a = 1.0f - r1 - r2;
    float b = r1;
    float c = r2;

    // Coordenadas baricéntricas --> coordenadas cartesianas
    Punto puntoAleatorio = v0 * a + v1 * b + v2 * c;

    Direccion crossProd = cross(v1 - v0, v2 - v0);
    float areaTriangulo = modulo(crossProd) * 0.5f;
    prob = 1.0f / areaTriangulo;

    return puntoAleatorio;
}

void Triangulo::diHola() const {
    cout << "Soy triangulo: p1 = " << this->v0 << endl;
}
