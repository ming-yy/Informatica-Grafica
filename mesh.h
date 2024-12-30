//*****************************************************************
// File:   mesh.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************
#include "primitiva.h"
#include "utilidades.h"
#include "triangulo.h"
#include "esfera.h"

class Mesh : public Primitiva {
public:
    vector<Triangulo> triangulos;
    vector<Punto> vertices;
    Esfera esferaLimite;

    Mesh();
    Mesh(const string rutaModelo, const float escala = 1.0f, const Punto& centro = Punto(0.0f, 0.0f, 0.0f),
        const float rotacionX = 0.0f, const bool invertirX = false, 
        const float rotacionY = 0.0f, const bool invertirY = false, 
        const float rotacionZ = 0.0f, const bool invertirZ = false,
        const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
        const string _material = "difuso", const RGB& _power = RGB(),
        const string rutaTextura = "");


    // Método para calcular la intersección entre un rayo y el triángulo
    // Algoritmo usado: Möller–Trumbore
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const override;
    
    // Método que devuelve "True" si y solo si el punto <p0> pertecene al triángulo.
    bool pertenece(const Punto& p0) const override;
    
    // Método que devuelve la normal de la primitiva en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;
    
    // Método que devuelve "True" si y solo si <punto> pertenece al triángulo
    // y además, es un punto lumínico del triángulo.
    bool puntoEsFuenteDeLuz(const Punto& punto) const override;
    
    // Método que devuelve un punto aleatorio del triángulo en UCS.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    Punto generarPuntoAleatorio(float& prob) const override;
    
    // Método que obtiene la posición del punto <pto> del triángulo en el
    // eje U de la textura correspondiente.
    float getEjeTexturaU(const Punto& pto) const override;
    
    // Método que obtiene la posición del punto <pto> del triángulo en el
    // eje V de la textura correspondiente.
    float getEjeTexturaV(const Punto& pto) const override;

    Esfera minimumBoundingSphere();

    
    // Debug
    void diHola() const override;
};