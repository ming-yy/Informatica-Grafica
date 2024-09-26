//*****************************************************************
// File:   planeta.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "planeta.h"

#define MARGEN_ERROR 10e-6
#define GRAD_A_RAD 3.1415926535898f/180

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
    
    calcularCoordLocEstacion();
    
    // No sirve para nada...
    // calcularVectoresCentroPlaneta();
}


void Planeta::calcularCoordLocEstacion(){
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
    // Base de la ciudad de referencia
    this->normal = normalizar(this->cref - this->centro);
    this->tangLong = normalizar(cross(this->normal, this->eje));
    this->tangLat = normalizar(this->eje);
}


Base Planeta::getBaseEstacion() {
    // estacionUCS = estacionRespCentro + centro (UCS)
    Punto estacionUCS = Punto(coordLocEstac) + this->centro;
    Direccion v1 = normalizar(estacionUCS - this->centro);
    Direccion v2 = normalizar(cross(v1, this->eje));
    Direccion v3 = normalizar(cross(v1, v2));
    
    // COMPROBAR QUE ESTAN EN EL ORDEN CORRECTO
    return Base(v2.coord, v3.coord, v1.coord);
}


Punto Planeta::estacionToUCS(const Base& ucs, const Punto& o) const {
    return Punto(coordLocEstac) + this->centro;
}


Direccion Planeta::getTrayectoria(const Planeta& pDestino, const Base& ucs, const Punto& o) {
    Punto origen = Punto(this->estacionToUCS(ucs, o));
    Punto destino = Punto(pDestino.estacionToUCS(ucs, o));
    Direccion trayec = Direccion(destino - origen);
    return normalizar(trayec);
}


bool Planeta::impactoOrEscape(const Direccion& trayectoria) {
    // Miramos la componente Z ("altura")
    return trayectoria.coord[2] > 0;
}


bool Planeta::interconexionPlanetaria(Planeta& pDest, const Base& ucs, const Punto& o) {
    Direccion trayUCS = Direccion(this->getTrayectoria(pDest, ucs, o));
    
    Base baseDest = Base(pDest.getBaseEstacion());
    
    // CAMBIAR pDest.cref POR LAS COORDENADAS DE LA ESTACION
    Direccion trayDest = Direccion(cambioBase(trayUCS, baseDest, pDest.cref));
    Base baseOrig = Base(this->getBaseEstacion());
    Direccion trayOrig = Direccion(cambioBase(trayUCS, baseOrig, this->cref));
    
    // Cambiamos de sentido porque pDestino necesita saber de "dónde viene" el cohete
    trayDest = trayDest * (-1);
    bool pDestEscape = pDest.impactoOrEscape(trayDest);
    bool pOrigEscape = this->impactoOrEscape(trayOrig);
    
    // DEBUG
    //cout << "Info Planeta origen:\n" << this << endl;
    //cout << "Info Planeta destino:\n" << pDest << endl;
    cout << "Trayectoria UCS: " << trayUCS
         << "\nTrayectoria destino: " << trayDest
         << "\nTrayectoria origen:" << trayOrig << endl;
    //cout << "Planeta origen, impacto (0) o escape (1)? " << pOrigEscape << endl;
    //cout << "Planeta destino, impacto (0) o escape (1)? " << pDestEscape << endl;
    
    return !pDestEscape and pOrigEscape;
}


ostream& operator<<(ostream& os, const Planeta& r)
{
    os << "Centro: " << r.centro << ",\nEje: " << r.eje << ",\nCiudad ref: "
       << r.cref << "\nRadio: " << r.radio << "\nNormal: " << r.normal
       << "\nTangente longitud: " << r.tangLong
       << "\nTangete latitud: " << r.tangLat << endl;

    return os;
}
