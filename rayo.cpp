//*****************************************************************
// File:   rayo.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "rayo.h"


Rayo::Rayo(const Direccion& dir, const Punto& origen) : d(dir), o(origen) {};

void Rayo::imprimir() const {
    std::cout << "Rayo - Direccion: " << d << ", Origen: " << o << std::endl;
}

ostream& operator<<(ostream& os, const Rayo& r)
{
    os << "Rayo - Direccion: " << r.d << ", Origen: " << r.o;
    return os;
}
