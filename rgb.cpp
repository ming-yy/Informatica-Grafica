//*****************************************************************
// File:   rgb.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "rgb.h"
#include <stdexcept>


RGB::RGB() : rgb({255.0f, 255.0f, 255.0f}) {}

RGB::RGB(const float& r, const float& g, const float& b): rgb({r, g, b}) {}

RGB::RGB(std::initializer_list<float> _rgb) {
    if (_rgb.size() == 3) {
        auto it_rgb = _rgb.begin();
        this->rgb = {*(it_rgb), *(it_rgb + 1), *(it_rgb + 2)};
    } else {
        throw std::invalid_argument("RGB no esta recibiendo un initializer_list de 3 valores.");
    }
}

RGB::RGB(const std::array<float, 3>& _rgb): rgb(_rgb) {}

