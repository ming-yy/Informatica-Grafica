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
#include "direccion.h"

template<typename T, std::size_t N>
using array = std::array<T, N>;

template<typename T>
using init_list = std::initializer_list<T>;

using std::ostream;

class Base {
public:
    array<array<float, 3>, 3> base;

    Base();
    Base(const Base& b);
    Base(init_list<init_list<float>> valores);
    Base(array<array<float, 3>, 3> valores);
    Base(const array<float, 3>& arr1, const array<float, 3>& arr2,
         const array<float, 3>& arr3);
    Base(const Direccion& arr1, const Direccion& arr2, const Direccion& arr3);
    
    // Función para mostrar por pantalla la base <b>
    friend ostream& operator<<(ostream& os, const Base& b);
};
