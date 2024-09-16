//*****************************************************************
// File:   matriz.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************
/*
#include "matriz.h"


template <std::size_t Filas, std::size_t Colum>
Matriz<Filas, Colum>::Matriz() {
    for (auto& fila : matriz) {
        fila.fill(0.0f);
    }
}

template <std::size_t Filas, std::size_t Colum>
Matriz<Filas, Colum>::Matriz(std::initializer_list<std::initializer_list<float>> valores) {
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

template <std::size_t Filas, std::size_t Colum>
std::size_t Matriz<Filas, Colum>::getFilas() const {
    return Filas;
}

template <std::size_t Filas, std::size_t Colum>
std::size_t Matriz<Filas, Colum>::getColumnas() const {
    return Colum;
}

template <std::size_t Filas, std::size_t Colum>
std::ostream& operator<<(std::ostream& os, const Matriz<Filas, Colum>& m) {
    for (const auto& fila : m.matriz) {
        for (const auto& valor : fila) {
            os << valor << " ";
        }
        os << std::endl;
    }
    return os;
}
*/
