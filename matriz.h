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
                os << valor << " ";
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


/*

#pragma once
#include <array>
#include <iostream>


template<std::size_t Filas, std::size_t Colum>
class Matriz {
public:
    std::array<std::array<float, Colum>, Filas> matriz;

    
    Matriz();
    Matriz(std::initializer_list<std::initializer_list<float>> valores);
    
    // Método que devuelve el número de filas que tiene la matriz
    std::size_t getFilas() const;
    
    // Método que devuelve el número de columnas que tiene la matriz
    std::size_t getColumnas() const;
    
    Matriz operator*(Matriz a);
    
    // Método para mostrar por pantalla la matriz
    friend std::ostream& operator<<(std::ostream& os, const Matriz& m);
};
*/
