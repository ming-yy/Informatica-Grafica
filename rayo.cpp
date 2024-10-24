//*****************************************************************
// File:   rayo.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "rayo.h"

// Constructor con parámetros
Rayo::Rayo(const Direccion& dir, const Punto& origen) : d(dir), o(origen) {};

// Constructor que acepta shared_ptr
Rayo::Rayo(const std::shared_ptr<Direccion>& dirPtr, const std::shared_ptr<Punto>& origenPtr)
        : d(*dirPtr), o(*origenPtr) {};

// Método para mostrar la información del rayo
void Rayo::imprimir() const {
    std::cout << "Rayo - Dirección: " << d << ", Origen: " << o << std::endl;
}
