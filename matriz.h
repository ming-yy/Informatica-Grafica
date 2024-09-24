//*****************************************************************
// File:   matriz.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include <iostream>
#include <initializer_list>
#include <cmath>

template<typename T, std::size_t N>
using array = std::array<T, N>;

template<typename T>
using init_list = std::initializer_list<T>;

using std::size_t;
using std::copy;
using std::cerr;
using std::endl;
using std::invalid_argument;
using std::abs;
using std::ostream;
using std::runtime_error;



template <size_t Filas, size_t Columnas>
class Matriz {
public:
    array<array<float, Columnas>, Filas> matriz;

    Matriz();
    Matriz(init_list<init_list<float>> valores);
    Matriz(array<array<float, Columnas>, Filas> valores);
    
    // Método para obtener el número de filas
    size_t getFilas() const { return Filas; }
    
    // Método para obtener el número de columnas
    size_t getColumnas() const { return Columnas; }

    // Método para multiplicar dos matrices: AxB * BxC = AxC
    template <size_t Columnas2>
    Matriz<Filas, Columnas2> operator*(const Matriz<Columnas, Columnas2>& otra) const;
    
    // Método que devuelve la matriz inversa
    Matriz<4, 4> inversa() const {
        Matriz<4, 4> inv;
        Matriz<4, 4> temp = *this; // Crear una copia temporal de la matriz actual

        // Matriz identidad
        for (size_t i = 0; i < 4; ++i) {
            inv.matriz[i][i] = 1.0f;
        }

        // Implementación de Gauss-Jordan para la inversa
        for (size_t i = 0; i < 4; ++i) {
            float diagElem = temp.matriz[i][i];
            if (diagElem == 0.0f) {
                throw runtime_error("La matriz es singular y no tiene inversa.");
            }

            // Dividir la fila actual por el elemento diagonal
            for (size_t j = 0; j < 4; ++j) {
                temp.matriz[i][j] /= diagElem;
                inv.matriz[i][j] /= diagElem;
            }

            // Hacer ceros en la columna actual para las otras filas
            for (size_t k = 0; k < 4; ++k) {
                if (k != i) {
                    float factor = temp.matriz[k][i];
                    for (size_t j = 0; j < 4; ++j) {
                        temp.matriz[k][j] -= factor * temp.matriz[i][j];
                        inv.matriz[k][j] -= factor * inv.matriz[i][j];
                    }
                }
            }
        }

        return inv;
    }
    
    // Función para mostrar por pantalla la matriz <m>
    friend ostream& operator<<(ostream& os, const Matriz& m) {
        for (const auto& fila : m.matriz) {
            os << "|";
            
            // Formatea para poder poner hasta nums de 3 cifras y que quede bonito
            for (const auto& valor : fila) {
                if(valor == 0.0f){
                    os << "  ";
                } else {
                    for(int i = log10(abs(valor)); i < 2; i++){
                        os << " "; 
                    }
                }
                
                os << " " << valor;
            }
            os << "|" << endl;
        }
        return os;
    }
};


// ----------------------------------------------------------------
//                          Implementación
// ----------------------------------------------------------------
template <size_t Filas, size_t Columnas>
Matriz<Filas, Columnas>::Matriz() {
    for (auto& fila : matriz) {
        fila.fill(0.0f);
    }
}


template <size_t Filas, size_t Columnas>
Matriz<Filas, Columnas>::Matriz(init_list<init_list<float>> valores) {
    try{
        if(Filas != valores.size() || Columnas != valores.begin()->size()){
            for (auto& fila : matriz) {
                fila.fill(0.0f);
            }
            throw invalid_argument("Error: Matriz e initializer_list no tienen las mismas filas/columnas.");
        }

        size_t i = 0;

        for (const auto& fila : valores) {
            size_t j = 0;
            for (const auto& valor : fila) {
                matriz[i][j] = valor;
                j++;
            }
            i++;
        }  
    } catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }
}


template <size_t Filas, size_t Columnas>
Matriz<Filas, Columnas>::Matriz(array<array<float, Columnas>, Filas> valores) {
    for (size_t i = 0; i < Filas; ++i) {
        copy(valores[i].begin(), valores[i].end(), matriz[i].begin());
    }
}


template <size_t Filas, size_t Columnas>
template <size_t Columnas2>
Matriz<Filas, Columnas2> Matriz<Filas, Columnas>::operator*(const Matriz<Columnas,
                                                            Columnas2>& otra) const {
    Matriz<Filas, Columnas2> resultado;

    for (size_t i = 0; i < Filas; ++i) {
        for (size_t j = 0; j < Columnas2; ++j) {
            resultado.matriz[i][j] = 0.0f; // Inicializa el valor en 0
            for (size_t k = 0; k < Columnas; ++k) {
                resultado.matriz[i][j] += this->matriz[i][k] * otra.matriz[k][j];
            }
        }
    }

    return resultado;
}

// Método para obtener la inversa de la matriz 4x4

