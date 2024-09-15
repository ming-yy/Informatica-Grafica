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
#include "coordenadas.h"
#include "punto.h"
#include "direccion.h"

//#include "punto.h"
//#include "direccion.h"


int main() {

    float arrCoord[3] = {4.44,5.55,6.66};


    // PRUEBA 1 - COORDENADAS
    std::shared_ptr<Coordenadas> coord1 = std::make_shared<Coordenadas> ();
    std::shared_ptr<Coordenadas> coord2 = std::make_shared<Coordenadas> (1.11,2.22,3.33);
    float arr1[3] = {4.44,5.55,6.66};
    std::shared_ptr<Coordenadas> coord3 = std::make_shared<Coordenadas> (arr1);
    std::cout << std::endl << "1 - COORDENADAS" << std::endl;
    std::cout << *coord1 << std::endl;
    std::cout << *coord2 << std::endl;
    std::cout << *coord3 << std::endl;

    // PRUEBA 2 - PUNTODIRECCION
    std::shared_ptr<PuntoDireccion> pd1 = std::make_shared<PuntoDireccion> ();
    std::shared_ptr<PuntoDireccion> pd2 = std::make_shared<PuntoDireccion> (1.11,2.22,3.33);
    std::shared_ptr<PuntoDireccion> pd3 = std::make_shared<PuntoDireccion> (arrCoord);
    std::cout << std::endl << "2 - PUNTODIRECCION" << std::endl;
    std::cout << *pd1 << std::endl;
    std::cout << *pd2 << std::endl;
    std::cout << *pd3 << std::endl;


    // PRUEBA 3 - PUNTO
    std::shared_ptr<Punto> punto1 = std::make_shared<Punto> ();
    std::shared_ptr<Punto> punto2 = std::make_shared<Punto> (1.111,2.222,3.333);
    std::shared_ptr<Punto> punto3 = std::make_shared<Punto> (arrCoord);
    std::cout << std::endl << "3 - PUNTO" << std::endl;
    std::cout << *punto1 << std::endl;
    std::cout << *punto2 << std::endl;
    std::cout << *punto3 << std::endl;

    // PRUEBA 4 - DIRECCION
    std::shared_ptr<Direccion> dir1 = std::make_shared<Direccion> ();
    std::shared_ptr<Direccion> dir2 = std::make_shared<Direccion> (1.111,2.222,3.333);
    std::shared_ptr<Direccion> dir3 = std::make_shared<Direccion> (arrCoord);

    std::cout << std::endl << "4 - DIRECCION" << std::endl;
    std::cout << *dir1 << std::endl;
    std::cout << *dir2 << std::endl;
    std::cout << *dir3 << std::endl;
    return 0;
}