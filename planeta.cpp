//*****************************************************************
// File:   planeta.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "planeta.h"

#define MARGEN_ERROR 10e-6
#define GRAD_A_RAD 3.1415926535898f/180


Planeta::Planeta(const Punto& _centro, const Direccion& _eje, const Punto& _cref):
                 centro(_centro), eje(_eje), cref(_cref) {
    radio = modulo(_cref - _centro);
    if (abs(modulo(_eje) - radio*2) > MARGEN_ERROR) {
        std::cout << "Eje: " << modulo(eje) << "\nRadio: " << radio << std::endl;
        throw std::invalid_argument("El eje del planeta (" + std::to_string(modulo(_eje)) +
                                 ") no es el doble del radio (" + std::to_string(radio) + ").");
    }
}


void Planeta::estacionToUCS(const float& inclinacion, const float& azimut, const Base& ucs, const Punto& o) {
    // Faltan comprobaciones rango de azimut e inclinación
    
    float sinAzim = static_cast<float>(sin(float(azimut * GRAD_A_RAD)));
    float sinIncl = static_cast<float>(sin(float(inclinacion * GRAD_A_RAD)));
    float cosAzim = static_cast<float>(cos(float(azimut * GRAD_A_RAD)));
    float cosIncl = static_cast<float>(cos(float(inclinacion * GRAD_A_RAD)));
    
    // Obtenemos coordenadas cartesianas de la estación en base al centro del planeta
    float x = this->radio * sinIncl * cosAzim;
    float y = this->radio * sinIncl * sinAzim;
    float z = this->radio * cosIncl;
    
    std::cout << "Coordenadas cartesianas: (" << x << ", " << y << ", " << z << ")" << std::endl;
    std::cout << "Coordenadas en UCS: \n" << cambioBase(Punto(x, y, z), ucs, o) << std::endl;
    
}


std::ostream& operator<<(std::ostream& os, const Planeta& r)
{
    os << "Centro: " << r.centro << ",\nEje: " << r.eje << ",\nCiudad ref: "
       << r.cref << "\nRadio: " << r.radio;

    return os;
}
