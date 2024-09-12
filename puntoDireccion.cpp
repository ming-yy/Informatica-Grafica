//*****************************************************************
// File:   puntoDireccion.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "puntoDireccion.h"

PuntoDireccion::PuntoDireccion() {
    coord[0] = 0.0f;
    coord[1] = 0.0f;
    coord[2] = 0.0f;
}

PuntoDireccion::PuntoDireccion(float x, float y, float z) {
    coord[0] = x;
    coord[1] = y;
    coord[2] = z;
}

PuntoDireccion::PuntoDireccion(float _coord[3]) {
    for (int i = 0; i < 3; ++i) {
        coord[i] = _coord[i];
    }
}

std::ostream& operator<<(std::ostream& os,const PuntoDireccion& r)
{
	os << "[" << r.coord[0] << ", " << r.coord[1] << ", " << r.coord[2] << "]";
	return os;
}
