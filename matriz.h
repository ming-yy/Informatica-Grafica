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


template <std::size_t Filas, std::size_t Columnas>
class Matriz {

public:
    std::array<std::array<float, Columnas>, Filas> matriz;

    Matriz();
    Matriz(std::initializer_list<std::initializer_list<float>> valores);
    Matriz(std::array<std::array<float, Columnas>, Filas> valores);
    
    // Método para obtener el número de filas
    std::size_t getFilas() const { return Filas; }
    
    // Método para obtener el número de columnas
    std::size_t getColumnas() const { return Columnas; }

    // Método para multiplicar dos matrices: AxB * BxC = AxC
    template <std::size_t Columnas2>
    Matriz<Filas, Columnas2> operator*(const Matriz<Columnas, Columnas2>& otra) const;
    
    // Función para mostrar por pantalla la matriz <m>
    friend std::ostream& operator<<(std::ostream& os, const Matriz& m) {
        for (const auto& fila : m.matriz) {
            os << "|";
            
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
            os << "|" << std::endl;
        }
        return os;
    }
};


// ----------------------------------------------------------------
//                          Implementación
// ----------------------------------------------------------------
template <std::size_t Filas, std::size_t Columnas>
Matriz<Filas, Columnas>::Matriz() {
    for (auto& fila : matriz) {
        fila.fill(0.0f);
    }
}


template <std::size_t Filas, std::size_t Columnas>
Matriz<Filas, Columnas>::Matriz(std::initializer_list<std::initializer_list<float>> valores) {
    try{
        if(Filas != valores.size() || Columnas != valores.begin()->size()){
            for (auto& fila : matriz) {
                fila.fill(0.0f);
            }
            throw std::invalid_argument("Error: Matriz e initializer_list no tienen las mismas filas/columnas.");
        }

        std::size_t i = 0;

        for (const auto& fila : valores) {
            std::size_t j = 0;
            for (const auto& valor : fila) {
                matriz[i][j] = valor;
                j++;
            }
            i++;
        }  
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
}


template <std::size_t Filas, std::size_t Columnas>
Matriz<Filas, Columnas>::Matriz(std::array<std::array<float, Columnas>, Filas> valores) {
    for (std::size_t i = 0; i < Filas; ++i) {
        std::copy(valores[i].begin(), valores[i].end(), matriz[i].begin());
    }
}


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
