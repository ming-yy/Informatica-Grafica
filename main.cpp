//*****************************************************************
// File:   main.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************


#include <iostream>
#include <memory>
#include <string>
#include "coordenadas.h"
#include "punto.h"
#include "direccion.h"
#include "matriz.h"


int main(int argc, char* argv[]) {
    // SORRY NO ME VA DE OTRA FORMA
    std::string test[2] = {"foo", "3"};
    
    //if(argc == 1){
    if (false) {
        printf("ERROR: Elige un numero de prueba.\n");
    } else if (test[1] == std::string("1")) {
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
    
    } else if (test[1] == std::string("2")) {
        float aux[3] = {0.111, 1.222, 2.333};
        
        // PRUEBA 1 - DIRECCIONES
        std::shared_ptr<Direccion> dir1 = std::make_shared<Direccion> (1.111,2.222,3.333);
        std::shared_ptr<Direccion> dir2 = std::make_shared<Direccion> (aux);
        std::shared_ptr<Direccion> dir3 = std::make_shared<Direccion> ();
        std::shared_ptr<Direccion> dirMod = std::make_shared<Direccion> (0.111, 0.222, 0.333);
        std::shared_ptr<Direccion> dirNor = std::make_shared<Direccion> (0, 0, 0);
        std::shared_ptr<Direccion> dirEsc = std::make_shared<Direccion> (1, 1, 1);
        std::shared_ptr<Direccion> dir90a = std::make_shared<Direccion> (1, 0, 0);
        std::shared_ptr<Direccion> dir90b = std::make_shared<Direccion> (0, 1, 0);
        
        std::cout << std::endl << "1 - DIRECCION OPERANDOS" << std::endl;
        std::cout << "Suma: " << *dir1 + *dir3 << std::endl;
        std::cout << "Resta: " << *dir1 - *dir2 << std::endl;

        // Multiplicacion y division de direcciones por escalares
        float escalar = 2.0f;
        try {
            std::cout << "Multiplicacion por " << escalar << ": " << *dir1 * escalar
                    << std::endl;
            std::cout << "Division por " << escalar << ": " << *dir1 / escalar
                    << std::endl;
            std::cout << "Division por 0 (debe lanzar excepcion): " << *dir1 / 0
                    << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
        }
        
        // Modulo
        std::cout << "Modulo: " << modulo(*dir1) << std::endl;
        std::cout << "Modulo tras resta: " << modulo(*dir1 - *dirMod) << std::endl;
        
        // Normalizacion
        try {
            std::cout << "Normalizacion de " << *dir1 << ":" << normalizar(*dir1) << std::endl;
            std::cout << "Normalizacion de " << *dirMod << ":" << normalizar(*dirMod) << std::endl;
            std::cout << "Normalizacion de " << *dir2 << ":" << normalizar(*dir2) << std::endl;
            std::cout << "Normalizacion de " << *dirNor << ":" << normalizar(*dirNor) << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
        }
        
        // Producto escalar y vectorial
        std::cout << "Producto escalar: " << dot(*dir1, *dir2) << std::endl;
        std::cout << "Producto escalar (0 grados): " << dot(*dir1, *dir1) << std::endl;
        std::cout << "Producto escalar (90 grados): " << dot(*dirNor, *dirEsc) << std::endl;
        std::cout << "Producto vectorial: " << cross(*dir1, *dir2) << std::endl;
        std::cout << "Producto vectorial (0 grados): " << cross(*dirEsc, *dirEsc) << std::endl;
        std::cout << "Producto vectorial (90 grados): " << cross(*dir90a, *dir90b) << std::endl;
        
        
        // PRUEBA 2 - PUNTOS
        std::shared_ptr<Punto> punto1 = std::make_shared<Punto>();
        std::shared_ptr<Punto> punto2 = std::make_shared<Punto>(aux);
        std::shared_ptr<Punto> punto3 = std::make_shared<Punto>(1.111, 2.222, 3.333);

        std::shared_ptr<Direccion> dir4 = std::make_shared<Direccion>(1.111, 2.222, 3.333);
        std::shared_ptr<Direccion> dir5 = std::make_shared<Direccion>(aux);

        std::cout << std::endl << "2 - PUNTO OPERANDOS" << std::endl;
        std::cout << "Suma con direccion: " << *punto1 + *dir4 << std::endl;
        std::cout << "Suma con direccion: " << *punto2 + *dir5 << std::endl;
        std::cout << "Resta con direccion: " << *punto3 - *dir4 << std::endl;

        // Multiplicacion y division de puntos por escalares
        try {
            std::cout << "Multiplicacion de punto por " << escalar << ": "
                        << (*punto1 * escalar) << std::endl;
            std::cout << "Division de punto por " << escalar << ": "
                        << (*punto3 / escalar) << std::endl;
            std::cout << "Division de punto por 0 (debe lanzar excepcion): "
                        << (*punto3 / 0) << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
        }

    } else if (test[1] == std::string("3")) {
        
        std::shared_ptr<Matriz<3, 3>> matriz1 = std::make_shared<Matriz<3, 3>> ();
        
        std::shared_ptr<Matriz<4, 4>> matriz4x4 = std::make_shared<Matriz<4, 4>>(
            std::initializer_list<std::initializer_list<float>>{
                {1.0f, 2.0f, 3.0f, 4.0f},
                {5.0f, 6.0f, 7.0f, 8.0f},
                {9.0f, 10.0f, 11.0f, 12.0f},
                {13.0f, 14.0f, 15.0f, 16.0f}
            }
        );
        
        std::shared_ptr<Matriz<3, 3>> matriz3x3 = std::make_shared<Matriz<3, 3>>(
            std::initializer_list<std::initializer_list<float>>{
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
            }
        );
        
        std::shared_ptr<Matriz<4, 3>> matriz4x3 = std::make_shared<Matriz<4, 3>>(
            std::initializer_list<std::initializer_list<float>>{
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
            }
        );

        
        std::cout << "Matriz 3x3:" << std::endl;
        std::cout << *matriz1 << std::endl;
        
        std::cout << "Matriz 4x4:" << std::endl;
        std::cout << *matriz4x4 << std::endl;
        
        std::cout << "Matriz 3x3:" << std::endl;
        std::cout << *matriz3x3 << std::endl;
        
        std::cout << "Matriz 4x3 (MIRAR Código):" << std::endl;
        std::cout << *matriz4x3 << std::endl;
    } else {
        printf("ERROR: No se ha encontrado el numero de prueba.\n");
    }

    
    return 0;
}
