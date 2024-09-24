//*****************************************************************
// File:   puntoDireccion.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "puntoDireccion.h"

template<typename T, std::size_t N>
using array = std::array<T, N>;

using std::ostream;

PuntoDireccion::PuntoDireccion(): coord({0.0f, 0.0f, 0.0f}) {}

PuntoDireccion::PuntoDireccion(float x, float y, float z) : coord({x, y, z}) {}

PuntoDireccion::PuntoDireccion(array<float,3> _coord) : coord(_coord) {}

ostream& operator<<(ostream& os, const PuntoDireccion& r)
{
    os << "[" << r.coord[0] << ", " << r.coord[1] << ", " << r.coord[2] << "]";

    return os;
}
