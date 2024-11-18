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
#include <initializer_list>

#define init_list std::initializer_list

class BSDFs {
public:
    RGB kd;
    RGB ks;
    RGB kt;

    BSDFs();
    BSDFs(const RGB& _color, const std::string _material);
    BSDFs(const RGB& _color, const RGB& _kd, const RGB& _ks, const RGB& _kt);
    BSDFs(const RGB& _color, std::array<float, 3> _kd, std::array<float, 3> _ks,
          std::array<float, 3> _kt);
    BSDFs(const RGB& color, const init_list<float> _kd, const init_list<float> _ks,
          const init_list<float> _kt);
    ~BSDFs() = default;

    // Operador de asignación por copia de objeto de la misma clase
    BSDFs& operator=(const BSDFs& d);
    
    friend std::ostream& operator<<(std::ostream& os, const BSDFs& r);
};
