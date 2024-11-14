//*****************************************************************
// File:   bsdfs.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4.3 de Informática Gráfica
//*****************************************************************
#include <map>
#include <array>

#define KD_i 0  // coef difuso
#define KS_i 1  // coef especular (reflexión)
#define KT_i 2  // coef transmitancia (refracción)

using FloatArray = std::array<float, 3>;
    
std::map<std::string, FloatArray> materiales{
    {"difuso", {0.9f, 0.0f, 0.0f}},
    {"cristal", {0.0f, 0.2f, 0.7f}},
    {"espejo", {0.0f, 0.9f, 0.0f}},
    {"plastico", {0.65f, 0.25f, 0.0f}},
}

BSDFs::BSDFs(const RGB& _color, const std::string _material) {
    FloatArray coefs = materiales[_material];
    kd = _color * coef[KD_i];
    ks = _color * coef[KS_i];
    kt = _color * coef[KT_i];
}


BSDFs::BSDFs(const RGB& _color, const RGB& _kd, const RGB& _ks, const RGB& _kt) {
    kd = _color * _kd;
    ks = _color * _ks;
    kt = _color * _kt;
}

