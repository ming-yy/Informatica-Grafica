//*****************************************************************
// File:   bsdfs.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4.3 de Informática Gráfica
//*****************************************************************
#pragma once
#include <string>
#include <array>
#include "rgb.h"
#include "utilidades.h"


constexpr int KD_i = 0;  // coef difuso
constexpr int KS_i = 1;  // coef especular (reflexión)
constexpr int KT_i = 2;  // coef transmitancia (refracción)


class BSDFs {
public:
    RGB kd;
    RGB ks;
    RGB kt;
    array<float, 3> sinEmision;

    BSDFs();
    BSDFs(const RGB& _color, const string _material);
    BSDFs(const RGB& _color, const RGB& _kd, const RGB& _ks, const RGB& _kt);
    BSDFs(const RGB& _color, const array<float, 3> _kd, const array<float, 3> _ks,
          const array<float, 3> _kt);
    ~BSDFs() = default;

    // Operador de asignación por copia de objeto de la misma clase
    BSDFs& operator=(const BSDFs& d);
    
    friend ostream& operator<<(ostream& os, const BSDFs& r);
};
