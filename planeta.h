//*****************************************************************
// File:   planeta.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>
#include <cmath>
#include "punto.h"
#include "direccion.h"
#include "base.h"
#include "matriz.h"
#include "transformaciones.h"

template<typename T, std::size_t N>
using array = std::array<T, N>;

using std::ostream;

class Planeta {
public:
    Punto centro;   // En UCS
    Direccion eje;  // Modulo debe ser doble del radio
    Punto cref;     // En UCS
    float radio;
    array<float, 2> estacion;  // [inclinacion, azimut] respecto a <cref>
    
    array<float, 3> coordLocEstac; // Coord locales de estación respecto al planeta
    Direccion normal;   // Respecto a la superficie de la estación y tiene módulo 1
    Direccion tangLong; // Respecto a superficie planeta y 90º con eje planeta
    Direccion tangLat;  // Respecto superficie planeta y 90º con <tangLong>
    
    // Constructor de Planeta
    Planeta(const Punto& _centro, const Direccion& _eje, const Punto& _cref,
            const float& inclinacion, const float& azimut);
    
    // Método que obtiene el sistema de coordenadas (base) de la estación planetaria.
    Base getBaseEstacion();
    
    // Método que ...
    Punto estacionToUCS(const Base& ucs, const Punto& o) const;
    
    // Método que dado este planeta como origen y el planeta <p> como destino, devuelve
    // la trayectoria entre sus respectivas estaciones en UCS
    Direccion getTrayectoria(const Planeta& pDestino, const Base& ucs, const Punto& o);
    
    // Método que determina si dada la trayectoria del lanzamiento, se va a impactar
    // contra el planeta (return True) o se va a escapar del planeta (return False).
    // PRECONDICION: <trayectoria> en base local de la estación.
    bool impactoOrEscape(const Direccion& trayectoria);
    
    // Función para mostrar por pantalla el planeta
    friend ostream& operator<<(ostream& os, const Planeta& pd);

private:
    // Obtiene las coordenadas cartesianas de la estación en base al centro del planeta
    void calcularLocEstacion();

    // Obtiene los 3 vectores respecto al CENTRO DEL PLANETA
    void calcularVectoresCentroPlaneta();

};
