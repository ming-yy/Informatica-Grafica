//*****************************************************************
// File:   planeta.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "planeta.h"

#define MARGEN_ERROR 10e-6
#define GRAD_A_RAD 3.1415926535898f/180

template<typename T>
using sh_ptr = std::shared_ptr<T>;

using std::to_string;
using std::invalid_argument;
using std::endl;
using std::ostream;
using std::cout;

Planeta::Planeta(const Punto& _centro, const Direccion& _eje, const Punto& _cref,
                 const float& _inclinacion, const float& _azimut):
                 centro(_centro), eje(_eje), cref(_cref), estacion{_inclinacion, _azimut} {
    // Faltan comprobaciones rango de azimut e inclinación
    estacion[0] = _inclinacion;
    estacion[1] = _azimut;
                     
    radio = modulo(_cref - _centro);
    if (abs(modulo(_eje) - radio*2) > MARGEN_ERROR) {
        throw invalid_argument("Error: eje del planeta (" + to_string(modulo(_eje)) +
                                 ") no es el doble del radio (" + to_string(radio) + ").");
    }
    
    calcularLocEstacion();
    
    calcularVectoresCentroPlaneta(); 
}

void Planeta::calcularLocEstacion(){
    float sinAzim, sinIncl, cosAzim, cosIncl;

    sinAzim = static_cast<float>(sin(float(estacion[1] * GRAD_A_RAD)));
    sinIncl = static_cast<float>(sin(float(estacion[0] * GRAD_A_RAD)));
    cosAzim = static_cast<float>(cos(float(estacion[1] * GRAD_A_RAD)));
    cosIncl = static_cast<float>(cos(float(estacion[0] * GRAD_A_RAD)));

    coordLocEstac[0] = this->radio * sinIncl * cosAzim;
    coordLocEstac[1] = this->radio * sinIncl * sinAzim;
    coordLocEstac[2] = this->radio * cosIncl;
}

void Planeta::calcularVectoresCentroPlaneta(){
    
    //this->normal = normalizar(this->cref - this->centro);
    //this->tangLong = normalizar(cross(this->normal, this->eje));
    //this->tangLat = normalizar(this->eje);
    
    sh_ptr<Punto> estacionEnLocal = std::make_shared<Punto>(coordLocEstac);
    this->normal = normalizar(*estacionEnLocal - this->centro);
    this->tangLong = normalizar(cross(this->normal, this->eje));
    this->tangLat = normalizar(cross(this->normal, this->tangLong));
}

Base Planeta::getBaseEstacion() {
    return Base(this->normal.coord, this->tangLong.coord, this->tangLat.coord);
}


Punto Planeta::estacionToUCS(const Base& ucs, const Punto& o) const {
    sh_ptr<Punto> estacion = std::make_shared<Punto>(
                                            cambioBase(Punto(coordLocEstac[0], coordLocEstac[1],                          coordLocEstac[2]), ucs, o));
    *estacion = *estacion + this->centro;
    cout << "Coordenadas cartesianas respecto al planeta: (" << coordLocEstac[0]
              << ", " << coordLocEstac[1] << ", " << coordLocEstac[2] << ")" << endl;
    cout << "Coordenadas en UCS: \n" << *estacion << endl;
    
    return *estacion;
}


Direccion Planeta::getTrayectoria(const Planeta& pDestino, const Base& ucs, const Punto& o) {
    sh_ptr<Punto> origen = std::make_shared<Punto>(this->estacionToUCS(ucs, o));
    sh_ptr<Punto> destino = std::make_shared<Punto>(pDestino.estacionToUCS(ucs, o));
    sh_ptr<Direccion> trayec = std::make_shared<Direccion>(*destino - *origen);
    return normalizar(*trayec);
}


bool Planeta::impactoOrEscape(const Direccion& trayectoria) {
    float prodEsc = dot(trayectoria, this->normal);
    return (prodEsc > 0);
}


ostream& operator<<(ostream& os, const Planeta& r)
{
    os << "Centro: " << r.centro << ",\nEje: " << r.eje << ",\nCiudad ref: "
       << r.cref << "\nRadio: " << r.radio << "\nNormal: " << r.normal
       << "\nTangente longitud: " << r.tangLong
       << "\nTangete latitud: " << r.tangLat << endl;

    return os;
}
