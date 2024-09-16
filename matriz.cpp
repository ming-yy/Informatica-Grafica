//*****************************************************************
// File:   matriz.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "matriz.h"

/*
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

// Función para multiplicar dos matrices
template<std::size_t N, std::size_t M, std::size_t P>
Matriz<N, P> multiplicar(const Matriz<N, M>& A, const Matriz<M, P>& B) {
    // Comprobar que las matrices son compatibles para la multiplicación
    if (A.getColumnas() != B.getFilas()) {
        throw std::invalid_argument("Número de columnas de A debe ser igual al número de filas de B.");
    }

    Matriz<N, P> C;

    for (std::size_t n = 0; n < N; ++n) {
        for (std::size_t p = 0; p < P; ++p) {
            float num = 0.0f;
            for (std::size_t m = 0; m < M; ++m) {
                num += A.matriz[n][m] * B.matriz[m][p];
            }
            C.matriz[n][p] = num;
        }
    }

    return C;
}
