//*****************************************************************
// File:   rayo.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include "punto.h"


class Rayo {
public:
    Direccion d;  // Dirección del rayo
    Punto o;      // Origen del rayo

    
    Rayo();
    Rayo(const Direccion& dir, const Punto& origen);

    // Método para mostrar la información del rayo
    void imprimir() const;
};
