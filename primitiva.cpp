//*****************************************************************
// File:   primitiva.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "primitiva.h"
#include "bsdfs.h"
#include <stdexcept>
#include <array>

Primitiva::Primitiva () {
    coeficientes = BSDFs();
    soyLuz = false;
}

Primitiva::Primitiva (const RGB& color, const std::string material, const bool _soyLuz) {
    coeficientes = BSDFs(color, material);
    soyLuz = _soyLuz;
}

Primitiva::Primitiva (const RGB& color,
                        const array<float, 3> kd, 
                        const array<float, 3> ks, 
                        const array<float, 3> kt,
                        const bool _soyLuz) {
    coeficientes = BSDFs(color, kd, ks, kt);
    soyLuz = _soyLuz;
}