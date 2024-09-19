//*****************************************************************
// File:   puntoDireccion.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>
#include <array>
#include "matriz.h"

class PuntoDireccion {
public:
    std::array<float, 3> coord;
    
    PuntoDireccion();
    PuntoDireccion(float x, float y, float z);
    PuntoDireccion(std::array<float, 3> _coord);
    
    // Método que devuelve las coordenadas homogéneas
    virtual Matriz<4,1> getCoordHomo() = 0;

    // Función para mostrar por pantalla el contenido del punto o dirección
    friend std::ostream& operator<<(std::ostream& os, const PuntoDireccion& pd);
};
