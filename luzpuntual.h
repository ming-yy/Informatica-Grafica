//*****************************************************************
// File:   luzpuntual.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#pragma once
#include "punto.h"
#include "direccion.h"


class LuzPuntual {
public:
    Punto c;
    Direccion p;
    
    
    LuzPuntual();
    LuzPuntual(Punto _c, Direccion _p = {1.0f, 1.0f, 1.0f});
};
