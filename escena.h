//*****************************************************************
// File:   escena.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include <vector>
#include "primitiva.h"
#include "rgb.h"
#include "luzpuntual.h"

class Escena {
public:
    std::vector<Primitiva*> primitivas;   // Array de los objetos de la escena
    std::vector<LuzPuntual> luces;   // Array de los objetos de la escena
    
    Escena();
    Escena(std::vector<Primitiva*> _primitivas, std::vector<LuzPuntual> _luces);
    
    // Método que devuelve "True" si y solo si hay intersección entre el rayo <rayo> y algún
    // objeto de la escena. En caso de haber intersección, la emisión del punto por el
    // que interseca el rayo <rayo> con el primer objeto que se encuentre en la escena es
    // devuelta en <resEmision> y el punto es devuelto en <ptoMasCerca>
    bool interseccion(const Rayo& rayo, RGB& resEmision, Punto& ptoMasCerca) const;
};
