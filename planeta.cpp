//*****************************************************************
// File:   planeta.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "planeta.h"

#define MARGEN_ERROR 10e-6


Planeta::Planeta(const Punto& _centro, const Direccion& _eje, const Punto& _cref):
                 centro(_centro), eje(_eje), cref(_cref) {
    radio = modulo(_cref - _centro);
    if (abs(modulo(_eje) - radio*2) > MARGEN_ERROR) {
        std::cout << "Eje: " << modulo(eje) << "\nRadio: " << radio << std::endl;
        throw std::invalid_argument("El eje del planeta (" + std::to_string(modulo(_eje)) +
                                 ") no es el doble del radio (" + std::to_string(radio) + ").");
    }
}

std::ostream& operator<<(std::ostream& os, const Planeta& r)
{
    os << "Centro: " << r.centro << ",\nEje: " << r.eje << ",\nCiudad ref: "
       << r.cref << "\nRadio: " << r.radio;

    return os;
}
