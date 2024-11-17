//*****************************************************************
// File:   bsdfs.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4.3 de Informática Gráfica
//*****************************************************************
#pragma once
#include "rgb.h"
#include <string>
#include <array>

using std::array;

class BSDFs {
public:
    RGB kd;
    RGB ks;
    RGB kt;

    BSDFs();
    BSDFs(const RGB& _color, const std::string _material);
    BSDFs(const RGB& _color, const RGB& _kd, const RGB& _ks, const RGB& _kt);
    BSDFs(const RGB& _color, array<float, 3> _kd, 
         array<float, 3> _ks, array<float, 3> _kt);
    ~BSDFs() = default;

        // Operador de asignación
    BSDFs& operator=(const BSDFs& d);
    
    friend std::ostream& operator<<(std::ostream& os, const BSDFs& r);
};