//*****************************************************************
// File:   transformaciones.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

// Matriz.h
#ifndef TRANSFORMACIONES_H
#define TRANSFORMACIONES_H

#include <array>
#include "matriz.h"

template <std::size_t Filas, std::size_t Columnas>
Matriz<Filas, Columnas> translate() {
    for (auto& fila : matriz) {
        fila.fill(0.0f);
    }
}

#endif // TRANSFORMACIONES_H