//*****************************************************************
// File:   bsdfs.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4.3 de Informática Gráfica
//*****************************************************************
#pragma once
#include "rgb.h"
#include <string>

class BSDFs {
public:
    RGB kd;
    RGB ks;
    RGB kt;

    BSDFs(RGB _color, string _material) const;
    BSDFs(RGB _color, RGB _kd, RGB _ks, RGB _kt) const;

    ~BSDFs() = default;
};