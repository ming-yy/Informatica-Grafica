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


class Planeta {
public:
    Punto centro;   // En UCS
    Direccion eje;  // Modulo debe ser doble del radio
    Punto cref;     // En UCS
    float radio;
    std::array<float, 2> estacion;  // [inclinacion, azimut] respecto a <cref>
    
    
    Planeta(const Punto& _centro, const Direccion& _eje, const Punto& _cref);
    
    //
    void estacionToUCS(const float& inclinacion, const float& azimut, const Base& ucs, const Punto& o);
    
    // Método que determina si dada la trayectoria del lanzamiento, se va a impactar contra
    // el planeta (return True) o se va a escapar del planeta (return False).
    // PRECONDICION: ambas direcciones están en la base centrada en la estacion.
    bool impactoOrEscape(const Direccion& trayectoria, const Direccion& normal);
    
    // Falta función que dadas 2 estaciones y 2 bases, devuelva si el lanzamiento es correcto
    // 1. Queremos saber si la dirección de lanzamiento (trayectoria) en una base "B" NORMALIZADO coincide
    //    con la dirección obtenida al restar (PuntoDestinoUCS - PuntoOrigenUCS) en base "B" y NORMALIZADO
    // 2. En origen comprobamos que es "escape" y en destino comprobamos que hay "impacto"
    
    // Función para mostrar por pantalla el planeta
    friend std::ostream& operator<<(std::ostream& os, const Planeta& pd);
};
