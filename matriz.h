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
private:
    std::array<std::array<float, Columnas>, Filas> matriz;

public:
    Matriz(); // Constructor por defecto
    Matriz(std::initializer_list<std::initializer_list<float>> valores);

    std::size_t getFilas() const { return Filas; }
    std::size_t getColumnas() const { return Columnas; }

    friend std::ostream& operator<<(std::ostream& os, const Matriz& m) {
        for (const auto& fila : m.matriz) {

            for (const auto& valor : fila) {
                for(int i = static_cast<int>(round(valor)) / 10; i < 2; i++){
                    os << " "; 
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

#endif // MATRIZ_H
