//*****************************************************************
// File:   primitiva.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "primitiva.h"
#include "bsdfs.h"
#include "bsdfs.cpp"
#include <stdexcept>
#include <array>


Primitiva::Primitiva (const RGB& color, const std::string material) {
    coeficientes = BSDFs(color, material);
}