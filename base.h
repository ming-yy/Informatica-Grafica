//*****************************************************************
// File:   base.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include <iostream>
#include <initializer_list>


class Base {
public:
    std::array<std::array<float, 3>, 3> base;

    Base();
    Base(std::initializer_list<std::initializer_list<float>> valores);
    Base(std::array<std::array<float, 3>, 3> valores);
    
    // Función para mostrar por pantalla la base <b>
    friend std::ostream& operator<<(std::ostream& os, const Base& b);
};
