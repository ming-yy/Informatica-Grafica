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
    
    // Función para mostrar por pantalla el planeta
    friend std::ostream& operator<<(std::ostream& os, const Planeta& pd);
};
