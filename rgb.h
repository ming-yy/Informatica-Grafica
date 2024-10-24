//*****************************************************************
// File:   rgb.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include <initializer_list>


class RGB {
public:
    std::array<float,3> rgb;

    RGB();
    RGB(const float& r, const float& g, const float& b);
    RGB(std::initializer_list<float> _rgb);
    RGB(const std::array<float, 3>& _rgb);
};
