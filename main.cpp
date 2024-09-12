//*****************************************************************
// File:   main.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************


// Definir base de datos punto y otra dirección
// Cada una con sus operadores
// Mostrar por pantalla

#include <iostream>
#include <memory>

#include "punto.h"
#include "direccion.h"


int main(int argc, char* argv[]) {

    std::shared_ptr<Punto> punto1 = std::make_shared<Punto> ();
    /*
    std::shared_ptr<Punto> punto2 = std::make_shared<Punto> (1.111,2.222,3.333);
    float coordPunto[3] = {1.111,2.222,3.333};
    std::shared_ptr<Punto> punto3 = std::make_shared<Punto> (coordPunto);

    std::shared_ptr<Direccion> dir1 = std::make_shared<Direccion> ();
    std::shared_ptr<Direccion> dir2 = std::make_shared<Direccion> (1.111,2.222,3.333);
    float coordDir[3] = {1.111,2.222,3.333};
    std::shared_ptr<Direccion> dir3 = std::make_shared<Direccion> (coordDir);

    std::cout << "Puntos:\n" << "\t" << punto1 << "\n\t" << punto2 << "\n\t" << punto3;
    std::cout << "Direcciones:\n" << "\t" << dir1 << "\n\t" << dir2 << "\n\t" << dir3;
*/
    return 0;
}