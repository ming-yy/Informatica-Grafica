//*****************************************************************
// File:   puntoDireccion.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "puntoDireccion.h"
#include "punto.h"

PuntoDireccion::PuntoDireccion() {
    coord = std::make_shared<Coordenadas> ();
}

PuntoDireccion::PuntoDireccion(float x, float y, float z) {
    coord = std::make_shared<Coordenadas> (x,y,z);
}

PuntoDireccion::PuntoDireccion(float _coord[3]) {
    coord = std::make_shared<Coordenadas> (_coord);
}

std::ostream& operator<<(std::ostream& os,const PuntoDireccion& r)
{
	os << *r.coord;

	return os;
}
