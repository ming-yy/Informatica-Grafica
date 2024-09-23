//*****************************************************************
// File:   planeta.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "planeta.h"

#define MARGEN_ERROR 10e-6
#define GRAD_A_RAD 3.1415926535898f/180


Planeta::Planeta(const Punto& _centro, const Direccion& _eje, const Punto& _cref,
                 const float& _inclinacion, const float& _azimut):
                 centro(_centro), eje(_eje), cref(_cref), estacion{_inclinacion, _azimut} {
    // Faltan comprobaciones rango de azimut e inclinación
    estacion[0] = _inclinacion;
    estacion[1] = _azimut;
                     
    radio = modulo(_cref - _centro);
    if (abs(modulo(_eje) - radio*2) > MARGEN_ERROR) {
        throw std::invalid_argument("Error: eje del planeta (" + std::to_string(modulo(_eje)) +
                                 ") no es el doble del radio (" + std::to_string(radio) + ").");
    }
}


void Planeta::getBaseEstacion() {
    this->normal = normalizar(this->cref - this->centro);
    this->tangLong = normalizar(cross(this->normal, this->eje));
    this->tangLat = normalizar(this->eje);
    
    std::cout << "Normal: " << this->normal << "\nTangente longitud: " << this->tangLong
              << "\nTangete latitud: " << this->tangLat << std::endl;
}


void Planeta::estacionToUCS(const Base& ucs, const Punto& o) {
    float sinAzim = static_cast<float>(sin(float(estacion[1] * GRAD_A_RAD)));
    float sinIncl = static_cast<float>(sin(float(estacion[0] * GRAD_A_RAD)));
    float cosAzim = static_cast<float>(cos(float(estacion[1] * GRAD_A_RAD)));
    float cosIncl = static_cast<float>(cos(float(estacion[0] * GRAD_A_RAD)));
    
    // Obtenemos coordenadas cartesianas de la estación en base al centro del planeta
    float x = this->radio * sinIncl * cosAzim;
    float y = this->radio * sinIncl * sinAzim;
    float z = this->radio * cosIncl;
    
    std::cout << "Coordenadas cartesianas respecto al planeta: ("
              << x << ", " << y << ", " << z << ")" << std::endl;
    std::cout << "Coordenadas en UCS: \n" << cambioBase(Punto(x, y, z), ucs, o) << std::endl;
}


Direccion getTrayectoria(const Planeta& p) {
    return Direccion();
}


bool Planeta::impactoOrEscape(const Direccion& trayectoria, const Direccion& normal) {
    float prodEsc = dot(trayectoria, normal);
    return (prodEsc > 0);
}


std::ostream& operator<<(std::ostream& os, const Planeta& r)
{
    os << "Centro: " << r.centro << ",\nEje: " << r.eje << ",\nCiudad ref: "
       << r.cref << "\nRadio: " << r.radio;

    return os;
}
