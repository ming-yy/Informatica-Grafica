//*****************************************************************
// File:   main.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************


#include <iostream>
#include <memory>
#include <string>
#include "base.h"
#include "punto.h"
#include "direccion.h"
#include "planeta.h"
#include "matriz.h"
#include "transformaciones.h"
#include "gestorPPM.h"


template<typename T>
using sh_ptr = std::shared_ptr<T>;

template<typename T, std::size_t N>
using array = std::array<T, N>;

template<typename T>
using init_list = std::initializer_list<T>;


using std::cout;
using std::cerr;
using std::endl;
using std::invalid_argument;
using std::string;



int main() {
    int test = 6;
    
    if (test == 1) {
        array<float, 3> arrCoord = {4.44,5.55,6.66};
        /*
        // PRUEBA 1.5 - PUNTODIRECCION
        // quitada porque ahora es abstracta y no se puede instanciar
        sh_ptr<PuntoDireccion> pd1 = std::make_shared<PuntoDireccion> ();
        sh_ptr<PuntoDireccion> pd2 = std::make_shared<PuntoDireccion> (1.11,2.22,3.33);
        sh_ptr<PuntoDireccion> pd3 = std::make_shared<PuntoDireccion> (arrCoord);
        cout << endl << "1.5 - PUNTODIRECCION" << endl;
        cout << *pd1 << endl;
        cout << *pd2 << endl;
        cout << *pd3 << endl;
        */

        // PRUEBA 2 - PUNTO
        sh_ptr<Punto> punto1 = std::make_shared<Punto> ();
        sh_ptr<Punto> punto2 = std::make_shared<Punto> (1.111,2.222,3.333);
        sh_ptr<Punto> punto3 = std::make_shared<Punto> (arrCoord);
        cout << endl << "2 - PUNTO" << endl;
        cout << *punto1 << endl;
        cout << *punto2 << endl;
        cout << *punto3 << endl;
        
        // PRUEBA 3 - DIRECCION
        sh_ptr<Direccion> dir1 = std::make_shared<Direccion> ();
        sh_ptr<Direccion> dir2 = std::make_shared<Direccion> (1.111,2.222,3.333);
        sh_ptr<Direccion> dir3 = std::make_shared<Direccion> (arrCoord);
        sh_ptr<Direccion> dirCopia = std::make_shared<Direccion> (*dir3);
        
        cout << endl << "3 - DIRECCION" << endl;
        cout << *dir1 << endl;
        cout << *dir2 << endl;
        cout << *dir3 << endl;
        cout << *dirCopia << endl;
    
    } else if (test == 2) {
        array<float, 3> arrAux2 = {0.111, 1.222, 2.333};

        
        // PRUEBA 1 - DIRECCIONES
        sh_ptr<Direccion> dir1 = std::make_shared<Direccion> (1.111,2.222,3.333);
        sh_ptr<Direccion> dir2 = std::make_shared<Direccion> (arrAux2);
        sh_ptr<Direccion> dir3 = std::make_shared<Direccion> ();
        sh_ptr<Direccion> dirMod = std::make_shared<Direccion> (0.111, 0.222, 0.333);
        sh_ptr<Direccion> dirNor = std::make_shared<Direccion> (0, 0, 0);
        sh_ptr<Direccion> dirEsc = std::make_shared<Direccion> (1, 1, 1);
        sh_ptr<Direccion> dir90a = std::make_shared<Direccion> (1, 0, 0);
        sh_ptr<Direccion> dir90b = std::make_shared<Direccion> (0, 1, 0);
        
        
        cout << endl << "1 - DIRECCION OPERANDOS" << endl;
        cout << "Suma: " << *dir1 + *dir3 << endl;
        cout << "Resta: " << *dir1 - *dir2 << endl;

        // Multiplicacion y division de direcciones por escalares
        float escalar = 2.0f;
        try {
            cout << "Multiplicacion por " << escalar << ": " << *dir1 * escalar
                    << endl;
            cout << "Division por " << escalar << ": " << *dir1 / escalar
                    << endl;
            cout << "Division por 0 (debe lanzar excepcion): " << *dir1 / 0
                    << endl;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }
        
        // Modulo
        cout << "Modulo: " << modulo(*dir1) << endl;
        cout << "Modulo tras resta: " << modulo(*dir1 - *dirMod) << endl;
        
        // Normalizacion
        try {
            cout << "Normalizacion de " << *dir1 << ":" << normalizar(*dir1) << endl;
            cout << "Normalizacion de " << *dirMod << ":" << normalizar(*dirMod) << endl;
            cout << "Normalizacion de " << *dir2 << ":" << normalizar(*dir2) << endl;
            cout << "Normalizacion de " << *dirNor << ":" << normalizar(*dirNor) << endl;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }
        
        // Producto escalar y vectorial
        cout << "Producto escalar: " << dot(*dir1, *dir2) << endl;
        cout << "Producto escalar (0 grados): " << dot(*dir1, *dir1) << endl;
        cout << "Producto escalar (90 grados): " << dot(*dirNor, *dirEsc) << endl;
        cout << "Producto vectorial: " << cross(*dir1, *dir2) << endl;
        cout << "Producto vectorial (0 grados): " << cross(*dirEsc, *dirEsc) << endl;
        cout << "Producto vectorial (90 grados): " << cross(*dir90a, *dir90b) << endl;
        
        
        // PRUEBA 2 - PUNTOS
        sh_ptr<Punto> punto1 = std::make_shared<Punto>();
        sh_ptr<Punto> punto2 = std::make_shared<Punto>(arrAux2);
        sh_ptr<Punto> punto3 = std::make_shared<Punto>(1.111, 2.222, 3.333);

        sh_ptr<Direccion> dir4 = std::make_shared<Direccion>(1.111, 2.222, 3.333);
        sh_ptr<Direccion> dir5 = std::make_shared<Direccion>(arrAux2);

        cout << endl << "2 - PUNTO OPERANDOS" << endl;
        cout << "Suma con direccion: " << *punto1 + *dir4 << endl;
        cout << "Suma con direccion: " << *punto2 + *dir5 << endl;
        cout << "Resta con direccion: " << *punto3 - *dir4 << endl;

        // Multiplicacion y division de puntos por escalares
        try {
            cout << "Multiplicacion de punto por " << escalar << ": "
                        << (*punto1 * escalar) << endl;
            cout << "Division de punto por " << escalar << ": "
                        << (*punto3 / escalar) << endl;
            cout << "Division de punto por 0 (debe lanzar excepcion): "
                        << (*punto3 / 0) << endl;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }

    } else if (test == 3) {
        sh_ptr<Matriz<3, 3>> matriz1 = std::make_shared<Matriz<3, 3>> ();
        sh_ptr<Matriz<4, 4>> matriz4x4 = std::make_shared<Matriz<4, 4>>(
            init_list<init_list<float>>{
                {1.0f, 2.0f, 3.0f, 4.0f},
                {5.0f, 6.0f, 7.0f, 8.0f},
                {9.0f, 10.0f, 11.0f, 12.0f},
                {13.0f, 14.0f, 15.0f, 16.0f}
            }
        );
        
        sh_ptr<Matriz<3, 3>> matriz3x3 = std::make_shared<Matriz<3, 3>>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
            }
        );
        
        sh_ptr<Matriz<4, 3>> matriz4x3 = std::make_shared<Matriz<4, 3>>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
            }
        );
        
        sh_ptr<Matriz<4, 4>> matrizInv = std::make_shared<Matriz<4, 4>>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f, 2.0f},
                {0.0f, 1.0f, 2.0f, 0.0f},
                {0.0f, 2.0f, 1.0f, 0.0f},
                {2.0f, 0.0f, 0.0f, 1.0f}
            }
        );

        cout << "Matriz 3x3 vacia:" << endl;
        cout << *matriz1 << endl;
        
        cout << "Matriz 4x4:" << endl;
        cout << *matriz4x4 << endl;
        
        cout << "Matriz 3x3:" << endl;
        cout << *matriz3x3 << endl;
        
        cout << "Matriz 4x3:" << endl;
        cout << *matriz4x3 << endl;

        array<array<float, 3>, 2> arrMult1 = {{{1.0f,2.0f,3.0f}, {4.0f,5.0f,6.0f}}};
        array<array<float, 2>, 3> arrMult2 = {{{10.0f,11.0f}, {20.0f,21.0f}, {30.0f,31.0f}}};

        sh_ptr<Matriz<2, 3>> matrizMult1 = std::make_shared<Matriz<2, 3>>(arrMult1);
        sh_ptr<Matriz<3, 2>> matrizMult2 = std::make_shared<Matriz<3, 2>>(arrMult2);

        sh_ptr<Matriz<2, 2>> resultado = std::make_shared<Matriz<2, 2>>(*matrizMult1 * *matrizMult2);

        cout << "Multiplicacion de m1:" << endl << *matrizMult1 << endl;
        cout << "y m2:" << endl << *matrizMult2 << endl;
        cout << "RESULTADO:" << endl << *resultado << endl;
        cout << "Inversa de m1: " << endl << (*matrizInv).inversa() << endl;

    } else if (test == 4) {
        sh_ptr<Punto> punto1 = std::make_shared<Punto> (1.111,2.222,3.333);
        sh_ptr<Punto> punto2 = std::make_shared<Punto> (1,2,3);
        sh_ptr<Punto> o1 = std::make_shared<Punto> (0,0,0);
        sh_ptr<Punto> o2 = std::make_shared<Punto> (1,2,3);
        float x1=3, y1=2, z1=1, d=90;
        
        sh_ptr<Base> b1 = std::make_shared<Base>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f},
                {0.0f, 2.0f, 0.0f},
                {0.0f, 0.0f, 3.0f},
            }
        );
        
        
        cout << "VAMOS A MOVER el punto1:" << endl << *punto1 << endl << endl;
        
        cout << "TRASLACION con x = " << x1 << ", y = " << y1 << ", z = " << z1 << endl;
        cout << "RESULTADO:" << endl << translate(*punto1, x1, y1, z1) << endl;
        
        cout << "ESCALADO con x = " << x1 << ", y = " << y1 << ", z = " << z1 << endl;
        cout << "RESULTADO:" << endl << scale(*punto1, x1, y1, z1) << endl;
        
        cout << "ROTACION en X con d = " << d << endl;
        cout << "RESULTADO:" << endl << rotateX(*punto1, d) << endl;
        
        cout << "ROTACION en Y con d = " << d << endl;
        cout << "RESULTADO:" << endl << rotateY(*punto1, d) << endl;
        
        cout << "ROTACION en Z con d = " << d << endl;
        cout << "RESULTADO:" << endl << rotateZ(*punto1, d) << endl;
        
        cout << "CAMBIOBASE de " << *punto2 << ":\n" << cambioBase(*punto2, *b1, *o1) << endl;
        
    } else if (test == 5) {
        sh_ptr<Base> base1 = std::make_shared<Base> ();
        
        sh_ptr<Base> base2 = std::make_shared<Base>(
             init_list<init_list<float>>{
                 {1.0f, 0.0f, 0.0f},
                 {0.0f, 1.0f, 0.0f},
                 {0.0f, 0.0f, 1.0f},
             }
        );
        
        array<array<float, 3>, 3> arrBase3 = {
            {{1.0f,2.0f,0.0f}, {1.0f,0.0f,5.0f}, {0.0f, 7.0f, 0.0f}}
        };
        sh_ptr<Base> base3 = std::make_shared<Base>(arrBase3);
        
        sh_ptr<Base> baseError = std::make_shared<Base>(
             init_list<init_list<float>>{
                 {1.0f, 2.0f, 3.0f, 4.0f},
                 {5.0f, 6.0f, 7.0f, 8.0f},
                 {9.0f, 10.0f, 11.0f, 12.0f},
                 {13.0f, 14.0f, 15.0f, 16.0f}
             }
        );
        
        cout << "Base constructor default:" << endl;
        cout << *base1 << endl;
        
        cout << "Base constructor inicializer_list:" << endl;
        cout << *base2 << endl;
        
        cout << "Base constructor array:" << endl;
        cout << *base3 << endl;
        
        cout << "Base dimensiones incorrectas:" << endl;
        cout << *baseError << endl;
        
    } else if (test == 6) {
        sh_ptr<Punto> centro1 = std::make_shared<Punto>(0,0,0);
        sh_ptr<Punto> cref1 = std::make_shared<Punto>(1,0,0);
        sh_ptr<Direccion> eje1 = std::make_shared<Direccion>(0,2,0);
        sh_ptr<Direccion> eje2 = std::make_shared<Direccion>(0,4,0);
        sh_ptr<Base> ucs = std::make_shared<Base>(
             init_list<init_list<float>>{
                 {1.0f, 0.0f, 0.0f},
                 {0.0f, 1.0f, 0.0f},
                 {0.0f, 0.0f, 1.0f},
             }
        );
        sh_ptr<Punto> origenUCS = std::make_shared<Punto>(0,0,0);
        
        cout << "--- Pruebas planetas ---" << endl;
        sh_ptr<Planeta> planeta1 = std::make_shared<Planeta>(*centro1, *eje1, *cref1, 45, 0);
        cout << "\nPlaneta1:\n" << *planeta1 << endl;
        
        try {
            sh_ptr<Planeta> planeta2 = std::make_shared<Planeta>(*centro1, *eje2, *cref1, 90, 90);
            cout << "\nPlaneta: " << *planeta2 << endl;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }
        
        
        sh_ptr<Punto> centro3 = std::make_shared<Punto>(5,0,0);
        sh_ptr<Punto> cref3 = std::make_shared<Punto>(3,0,0);
        sh_ptr<Direccion> eje3 = std::make_shared<Direccion>(0,4,0);
        sh_ptr<Planeta> planeta3 = std::make_shared<Planeta>(*centro3, *eje3, *cref3, -45, 0);
        cout << "\nPlaneta3:\n" << *planeta3 << endl;
        
        
        cout << "--- Pruebas interconexion planetaria ---" << endl;
        bool resultado = planeta1->interconexionPlanetaria(*planeta3, *ucs, *origenUCS);
        cout << "Interconexion planetaria exitosa? " << resultado << endl;
        
    } else if (test == 7) {
        sh_ptr<Base> ejemplo_print = std::make_shared<Base>(
             init_list<init_list<float>>{
                { 1.111, -2.22, 333.333 },
                { 10.1, 25.555, -0.333 },
                { -1123.456, 0.0, 7.89912 }
             }
        );

        cout << *ejemplo_print;

        sh_ptr<Matriz<4, 4>> ejemplo_print2 = std::make_shared<Matriz<4, 4>>(
            init_list<init_list<float>>{
                { 1.111, -2.22, 333.333, 0.0 },
                { 10.1, 25.555, -0.333, 44.44 },
                { -123.456, 0.0, 7.89, 3.14159 },
                { -13.456, 100.0, -7.89, 3.14159 }
            }
        );

        cout << *ejemplo_print2;
        
    } else if (test == 8){
        string nombreFichero = "ppms/forest_path.ppm";
        return transformarFicheroPPM(nombreFichero);
        
    } else {
        printf("ERROR: No se ha encontrado el numero de prueba.\n");
    }
        
    return 0;
}
