//*****************************************************************
// File:   primitiva.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************
#include "primitiva.h"

// Clase virtual que todas las primitivas geométrica deben heredar
// Quizás también habría que hacer que intersección() sea virtual
Primitiva::Primitiva() {}

void Primitiva::interseccion(const Rayo& rayo, std::vector<Punto>& ptos,
                         std::array<float, 3>& emision) const{}