//*****************************************************************
// File:   matriz.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

// Matriz.h
#ifndef MATRIZ_H
#define MATRIZ_H

#include <array>
#include <iostream>
#include <initializer_list>
#include <cmath>

template <std::size_t Filas, std::size_t Columnas>
class Matriz {

public:
    std::array<std::array<float, Columnas>, Filas> matriz;

    Matriz(); // Constructor por defecto
    Matriz(std::initializer_list<std::initializer_list<float>> valores);
    Matriz(float valores[Filas][Columnas]);


    std::size_t getFilas() const { return Filas; }
    std::size_t getColumnas() const { return Columnas; }

    // Sobrecarga del operador de multiplicación como un método miembro
    template <std::size_t Columnas2>
    Matriz<Filas, Columnas2> operator*(const Matriz<Columnas, Columnas2>& otra) const;

    friend std::ostream& operator<<(std::ostream& os, const Matriz& m) {
        for (const auto& fila : m.matriz) {

            // Formatea para poder poner hasta nums de 3 cifras y que quede bonito
            for (const auto& valor : fila) {
                if(valor == 0.0f){
                    os << "  ";
                } else {
                    for(int i = log10(std::abs(valor)); i < 2; i++){
                        os << " "; 
                    }
                }
                
                os << " " << valor;
            }
            os << std::endl;
        }
        return os;
    }


};

// Implementación de la plantilla
template <std::size_t Filas, std::size_t Columnas>
Matriz<Filas, Columnas>::Matriz() {
    for (auto& fila : matriz) {
        fila.fill(0.0f);
    }
}

template <std::size_t Filas, std::size_t Columnas>
Matriz<Filas, Columnas>::Matriz(std::initializer_list<std::initializer_list<float>> valores) {
    std::size_t i = 0;

    // Para rellenar primero todo de ceros,
    // por si luego no hay valores para toda la matriz
    for (auto& fila : matriz) {
        fila.fill(0.0f);
    }

    for (const auto& fila : valores) {
        std::size_t j = 0;
        for (const auto& valor : fila) {
            matriz[i][j] = valor;
            j++;
        }
        i++;
    }    
    
}

template <std::size_t Filas, std::size_t Columnas>
Matriz<Filas, Columnas>::Matriz(float valores[Filas][Columnas]) {
    for (std::size_t i = 0; i < Filas; ++i) {
        std::copy(std::begin(valores[i]), std::end(valores[i]), matriz[i].begin());
    }
}

// Implementación de la sobrecarga del operador de multiplicación
template <std::size_t Filas, std::size_t Columnas>
template <std::size_t Columnas2>
Matriz<Filas, Columnas2> Matriz<Filas, Columnas>::operator*(const Matriz<Columnas, Columnas2>& otra) const {
    Matriz<Filas, Columnas2> resultado;

    for (std::size_t i = 0; i < Filas; ++i) {
        for (std::size_t j = 0; j < Columnas2; ++j) {
            resultado.matriz[i][j] = 0.0f; // Inicializa el valor en 0
            for (std::size_t k = 0; k < Columnas; ++k) {
                resultado.matriz[i][j] += this->matriz[i][k] * otra.matriz[k][j];
            }
        }
    }

    return resultado;
}


#endif // MATRIZ_H
