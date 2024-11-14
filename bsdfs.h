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

    BSDFs(const RGB& _color, const std::string _material);
    BSDFs(const RGB& _color, const RGB& _kd, const RGB& _ks, const RGB& _kt);

    ~BSDFs() = default;
};